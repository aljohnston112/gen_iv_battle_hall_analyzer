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
                void(const ParameterType&, std::promise<PromiseResult>&&)
            >& function,
            const ParameterType& data
        ) {
            auto promise = std::promise<PromiseResult>{};
            futures.emplace_back(promise.get_future());
            addTask(
                [
                    &function = function,
                    &namedSeries = data,
                    promise = std::move(promise)
                ] mutable {
                    function(
                        namedSeries,
                        std::move(promise)
                    );
                }
            );
        }

        template <
            class PromiseResult,
            class InputDataContainer,
            class InputDataType
        >
        std::vector<PromiseResult> createAndRunTasks(
            std::function<
                void(const InputDataType&, std::promise<PromiseResult>&&)
            >&& function,
            const InputDataContainer& allData
        ) {
            std::vector<PromiseResult> results{};
            std::vector<std::future<PromiseResult>> futures;
            for (const InputDataType& data : allData) {
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
            class InputDataContainer,
            class InputDataType,
            std::size_t N,
            typename... TupleTypes
        >
        std::tuple<std::array<TupleTypes, N>...> createAndRunTasks2(
            auto&& function,
            const InputDataContainer& allData
        ) {
            std::tuple<std::array<TupleTypes, N>...> results{};
            std::vector<std::future<std::tuple<TupleTypes...>>> futures;
            for (const InputDataType& data : allData) {
                submit(
                    futures,
                    function,
                    data
                );
            }
            for (size_t i = 0; i < futures.size(); i++) {
                [&results, i]<std::size_t... Is>(
                    std::index_sequence<Is...>,
                    std::tuple<TupleTypes...> tuple_values
                ) {
                        ((std::get<Is>(results)[i] =
                                std::get<Is>(tuple_values)),
                            ...
                        );
                    }(std::make_index_sequence<N>{},
                      futures[i].get());
            }
            return results;
        }
    };
}

#endif //WAVELET_THREADPOOL_H
