#include <future>

#include "thread_pool.h"

#include "config.h"

namespace thread_pool {
    ThreadPool::ThreadPool(const int numThreads) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back(
                [this] {
                    while (true) {
                        Function task;
                        {
                            std::unique_lock lock(mutex);
                            task_condition.wait(
                                lock,
                                [this] {
                                    return stop || !tasks.empty();
                                }
                            );
                            if (tasks.empty() && stop) {
                                return;
                            }
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                }
            );
        }
    }

    void ThreadPool::addTask(Function&& function) {
        {
            std::lock_guard lock(mutex);
            tasks.emplace(std::move(function));
            task_condition.notify_one();
        }
    }

    void ThreadPool::addTasks(std::vector<Function>& functions) {
        {
            std::lock_guard lock(mutex);
            for (Function& function : functions) {
                tasks.emplace(std::move(function));
            }
            task_condition.notify_all();
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::lock_guard lock(mutex);
            stop = true;
            task_condition.notify_all();
        }
        for (std::jthread& thread : threads) {
            thread.join();
        }
    }

    unsigned int ThreadPool::getNumberOfThreads() {
        static unsigned int numThreads = std::thread::hardware_concurrency();
        if (MULTI_THREADED) {
            return numThreads;
        }
        return 1;
    }

    std::shared_ptr<ThreadPool> ThreadPool::getCPUWorkInstance() {
        static auto thread_pool =
            std::make_shared<ThreadPool>(getNumberOfThreads());
        return thread_pool;
    }

    std::shared_ptr<ThreadPool> ThreadPool::getDiskReadInstance() {
        static auto thread_pool =
            std::make_shared<ThreadPool>(getNumberOfThreads());
        return thread_pool;
    }

    std::shared_ptr<ThreadPool> ThreadPool::getDiskWriteInstance() {
        static auto thread_pool =
            std::make_shared<ThreadPool>(getNumberOfThreads());
        return thread_pool;
    }
}
