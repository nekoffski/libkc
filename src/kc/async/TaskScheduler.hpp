#pragma once

#include <functional>
#include <thread>
#include <vector>
#include <future>

#include "kc/core/Scope.hpp"
#include "kc/containers/ThreadSafeQueue.hpp"

#include "Future.hpp"

namespace kc::async {

class TaskScheduler {
    using Task = std::function<void()>;

    class TaskQueue : public containers::ThreadSafeQueue<Task> {
    public:
        template <typename T, typename F>
        std::optional<Future<T>> tryPush(F&& task) {
            auto context = std::make_shared<Context<T>>();
            {
                std::unique_lock lock(m_mutex, std::try_to_lock);
                if (not lock) return {};

                m_buffer.push_back(
                  [promise = Promise<T>(context), f = std::move(task)]() mutable {
                      handleTask(std::move(f), promise);
                  }
                );
            }
            m_ready.notify_one();
            return Future<T>(context);
        }

        template <typename T, typename F> Future<T> push(F&& task) {
            auto context = std::make_shared<Context<T>>();
            {
                std::unique_lock lock(m_mutex);
                m_buffer.push_back(
                  [promise = Promise<T>(context), f = std::move(task)]() mutable {
                      handleTask(std::move(f), promise);
                  }
                );
            }
            m_ready.notify_one();
            return Future<T>(context);
        }

    private:
        template <typename F, typename T>
        static void handleTask(F&& task, Promise<T>& promise) {
            if constexpr (std::is_void_v<std::result_of_t<F()>>) {
                task();
                promise.setValue();
            } else {
                promise.setValue(task());
            }
        }
    };

public:
    explicit TaskScheduler(int workerCount = std::thread::hardware_concurrency());
    ~TaskScheduler();

    int getWorkerCount() const;

    template <typename F> auto call(F&& f) -> Future<std::result_of_t<F()>> {
        using R                    = std::result_of_t<F()>;
        static constexpr int scale = 2;

        auto startQueueIndex = m_currentWorkerId++;

        for (int i = 0; i < m_workerCount * scale; ++i) {
            const auto index = (startQueueIndex + i) % m_workerCount;
            if (auto future = m_queues[index].tryPush<R>(std::forward<F>(f)); future)
                return std::move(future.value());
        }

        return m_queues[startQueueIndex % m_workerCount].push<R>(std::forward<F>(f));
    }

private:
    std::optional<Task> findTask(int workerIndex);
    void run(int index);

    const int m_workerCount;
    std::vector<std::thread> m_workers;
    std::vector<TaskQueue> m_queues;

    std::atomic_int64_t m_currentWorkerId;
};

}  // namespace kc::async