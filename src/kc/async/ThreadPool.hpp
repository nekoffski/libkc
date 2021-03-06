#pragma once

#include <chrono>
#include <functional>
#include <future>
#include <thread>
#include <vector>

#include "Future.hpp"
#include "kc/containers/AtomicQueue.hpp"
#include "kc/core/Log.h"
#include "kc/core/Macros.h"

namespace kc::async {

class ThreadPool {
    using Job = std::function<void()>;

   public:
    explicit ThreadPool(const std::size_t size = defaultSize);

    ~ThreadPool();

    std::size_t getSize() const;

    template <typename F, typename... Args>
    Future<typename std::result_of<F(Args...)>::type> callAsync(F&& func, Args&&... args) {
        using ReturnType = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(func), std::forward<Args>(args)...)
        );
        auto res = task->get_future();

        m_jobs.enqueue([task]() { (*task)(); });

        return Future<ReturnType>(std::move(res));
    }

    template <typename F, typename... Args>
    void loopParallel(const int iterations, F&& function, Args&&... args) {
        const auto threadCount = getSize();

        ASSERT(iterations > 0, "iterations <= 0");
        ASSERT(threadCount > 0, "thread count <= 0");

        int batch = iterations / threadCount;

        std::vector<Future<void>> futures;
        futures.reserve(threadCount);

        for (int i = 0; i < threadCount; ++i) {
            int beginIndex = i * batch;
            int endIndex   = beginIndex + batch;

            if (i == threadCount - 1) endIndex += iterations % batch;

            futures.push_back(callAsync(function, beginIndex, endIndex));
        }
        wait(futures);
    }

    void stop();

    static constexpr std::size_t defaultSize = 4;

   private:
    void threadWorker();

    containers::AtomicQueue<Job> m_jobs;
    std::vector<std::jthread> m_threads;
    std::atomic_bool m_active;
    std::size_t m_size;
};
}  // namespace kc::async
