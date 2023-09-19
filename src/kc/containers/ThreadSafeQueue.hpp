#pragma once

#include <atomic>
#include <deque>
#include <mutex>
#include <condition_variable>
#include <optional>

namespace kc::containers {

// TODO: support for non r-values
template <typename T> class ThreadSafeQueue {
   public:
    explicit ThreadSafeQueue() : m_done(false) {}

    bool tryPush(T&& element) {
        {
            std::unique_lock lock(m_mutex, std::try_to_lock);
            if (not lock) return false;
            m_buffer.push_back(std::move(element));
        }
        m_ready.notify_one();
        return true;
    }

    void push(T&& element) {
        {
            std::unique_lock lock(m_mutex);
            m_buffer.push_back(std::move(element));
        }
        m_ready.notify_one();
    }

    int getSize() const {
        std::unique_lock lock(m_mutex);
        return m_buffer.size();
    }

    std::optional<T> tryPop() {
        std::unique_lock lock(m_mutex, std::try_to_lock);
        if (not lock || m_buffer.empty()) return {};

        return getFrontElement();
    }

    std::optional<T> pop() {
        std::unique_lock lock(m_mutex);
        while (m_buffer.empty() && not m_done) m_ready.wait(lock);

        if (m_buffer.empty()) return {};

        return getFrontElement();
    }

    bool isDone() const { return m_done; }

    void done() {
        m_done = true;
        m_ready.notify_all();
    }

   protected:
    T getFrontElement() {
        auto element = std::move(m_buffer.front());
        m_buffer.pop_front();
        return element;
    }

    std::atomic_bool m_done;

    mutable std::mutex m_mutex;
    std::condition_variable m_ready;

    std::deque<T> m_buffer;
};

}  // namespace kc::containers
