#pragma once

#include <chrono>
#include <functional>
#include <future>
#include <memory>
#include <mutex>

using namespace std::chrono_literals;

namespace kc::async {

struct TaskExecutor {
    class Timer {
       public:
        void sleep(std::chrono::milliseconds delay) { std::this_thread::sleep_for(delay); }

        auto now() { return m_clock.now(); }

       private:
        std::chrono::steady_clock m_clock;
    };

    Timer getTimer() { return Timer{}; }

    struct Factory {
        virtual std::unique_ptr<TaskExecutor> create() = 0;
    };

    virtual void runAsync(std::function<void()>&& task) = 0;
    virtual void waitForAll()                           = 0;
    virtual void clearFinishedTasks()                   = 0;
};

class AsyncFutureTaskExecutor : public TaskExecutor {
   public:
    struct Factory : TaskExecutor::Factory {
        std::unique_ptr<TaskExecutor> create() override {
            return std::make_unique<AsyncFutureTaskExecutor>();
        }
    };

    void runAsync(std::function<void()>&& task) override {
        std::scoped_lock lock{m_mutex};
        m_futures.emplace_back(std::async(std::launch::async, task));
    }

    void waitForAll() override {
        std::scoped_lock lock{m_mutex};
        for (auto& future : m_futures) future.wait();
        m_futures.clear();
    }

    void clearFinishedTasks() override {
        std::scoped_lock lock{m_mutex};
        std::erase_if(m_futures, [](auto& future) -> bool {
            return future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
        });
    }

   private:
    std::mutex m_mutex;
    std::vector<std::future<void>> m_futures;
};

}  // namespace kc::async