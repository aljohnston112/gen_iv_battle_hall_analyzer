#ifndef WAVELET_THREADPOOL_H
#define WAVELET_THREADPOOL_H

#include <condition_variable>
#include <functional>
#include <queue>
#include <thread>

namespace thread_pool {
    using Function = std::move_only_function<void()>;

    class ThreadPool {
        std::vector<std::jthread> threads{};

        std::mutex mutex;
        std::queue<Function> tasks;

        std::condition_variable task_condition;
        bool stop = false;

        static unsigned int getNumberOfThreads();

    public:
        explicit ThreadPool(int numThreads);

        static std::shared_ptr<ThreadPool> getCPUWorkInstance();

        static std::shared_ptr<ThreadPool> getDiskReadInstance();

        static std::shared_ptr<ThreadPool> getDiskWriteInstance();

        void addTasks(std::vector<Function>& functions);

        void addTask(Function&& function);

        ~ThreadPool();

        template <
            class PromiseResult,
            class ParameterType
        >
        void submit(
            std::vector<std::future<PromiseResult>>& futures,
            const std::function<
                void(ParameterType&, std::promise<PromiseResult>&&)
            >& function,
            ParameterType& data
        ) {
            auto promise = std::promise<PromiseResult>{};
            futures.emplace_back(promise.get_future());
            addTask(
                [
                    &function = function,
                    &data = data,
                    promise = std::move(promise)
                ] mutable {
                    function(
                        data,
                        std::move(promise)
                    );
                }
            );
        }

        template <
            class PromiseResult,
            class DataContainer,
            class ParameterType
        >
        auto createAndRunTasks(
            std::function<
                void(ParameterType&, std::promise<PromiseResult>&&)
            >&& function,
            DataContainer& allData
        ) {
            std::vector<PromiseResult> results{};
            std::vector<std::future<PromiseResult>> futures;
            for (ParameterType& data : allData) {
                submit(
                    futures,
                    function,
                    data
                );
            }
            for (auto& future : futures) {
                auto result = future.get();
                results.emplace_back(std::move(result));
            }
            return results;
        }

        template <
            class PromiseResult,
            class DataContainer,
            class ParameterType
        >
        auto runTasksAndFlattenMaps(
            std::function<
                void(
                    ParameterType&,
                    std::promise<PromiseResult>&&
                )
            >&& function,
            DataContainer& allData
        ) {
            PromiseResult results{};
            std::vector<std::future<PromiseResult>> futures;
            for (ParameterType& data : allData) {
                submit(
                    futures,
                    function,
                    data
                );
            }
            for (auto& future : futures) {
                auto result = future.get();
                for (const auto& [k, v]: result) {
                    results[std::move(k)] = std::move(v);
                }
            }
            return results;
        }
    };
}

#endif //WAVELET_THREADPOOL_H
