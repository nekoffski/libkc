#pragma once

#include <optional>
#include <functional>
#include <thread>
#include <vector>
#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>

namespace kc::async {

class TaskScheduler {
    class WorkQueue {
       public:
        using Task = std::function<void()>;

        explicit WorkQueue() : m_done(false) {}

        bool tryPush(Task&& task) {
            {
                std::unique_lock lock(m_mutex, std::try_to_lock);
                if (not lock) return false;
                m_tasks.push_back(std::move(task));
            }
            m_ready.notify_one();
            return true;
        }

        std::optional<Task> tryPop() {
            std::unique_lock lock(m_mutex, std::try_to_lock);
            if (not lock || m_tasks.empty()) return {};

            return getFrontTask();
        }

        std::optional<Task> pop() {
            std::unique_lock lock(m_mutex);
            while (m_tasks.empty() && not m_done) m_ready.wait(lock);

            if (m_tasks.empty()) return {};

            return getFrontTask();
        }

        bool isDone() const { return m_done; }

        void done() {
            m_done = true;
            m_ready.notify_all();
        }

       private:
        Task getFrontTask() {
            auto task = std::move(m_tasks.front());
            m_tasks.pop_front();
            return task;
        }

        std::atomic_bool m_done;

        std::mutex m_mutex;
        std::condition_variable m_ready;

        std::deque<Task> m_tasks;
    };

   public:
    explicit TaskScheduler(int workerCount = std::thread::hardware_concurrency())
        : m_workerCount(workerCount), m_queues(workerCount), m_currentWorkerId(0l) {
        m_workers.reserve(workerCount);
        for (int i = 0; i < workerCount; ++i) m_workers.emplace_back([&, i]() { run(i); });
    }

    ~TaskScheduler() {
        for (auto& queue : m_queues) queue.done();
        for (auto& worker : m_workers) worker.join();
    }

    template <typename F> void callAsync(F&& f) {
        static constexpr int scale = 2;

        auto startQueueIndex = m_currentWorkerId++;

        for (int i = 0; i < m_workerCount * scale; ++i)
            if (m_queues[startQueueIndex + i].tryPush(std::forward<F>(f))) return;

        m_queues[queueIndex % m_workerCount].push(std::forward<F>(f));
    }

   private:
    std::optional<WorkQueue::Task> findTask(int workerIndex) {
        // try to steal task if there is nothing for us at this moment
        for (int i = 0; i < m_workerCount; ++i)
            if (auto task = m_queues[(workerIndex + i) % m_workerCount].tryPop(); task) return task;
        return m_queues[workerIndex].pop();
    }

    void run(int index) {
        while (true) {
            if (auto task = findTask(index); task) [[likely]]
                std::invoke(task.value());
            else
                break;
        }
    }

    const int m_workerCount;
    std::vector<std::thread> m_workers;
    std::vector<WorkQueue> m_queues;

    std::atomic_int64_t m_currentWorkerId;
};

}  // namespace kc::async