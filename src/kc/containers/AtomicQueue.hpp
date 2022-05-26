#pragma once

#include <mutex>
#include <optional>
#include <queue>

namespace kc::containers {

template <typename T> class AtomicQueue {
   public:
    std::size_t size() const {
        std::scoped_lock guard{m_queueMutex};
        return m_queue.size();
    }

    bool isEmpty() const {
        std::scoped_lock guard{m_queueMutex};
        return m_queue.empty();
    }

    void enqueue(T&& value) {
        std::scoped_lock guard{m_queueMutex};
        m_queue.push(value);
    }

    void enqueue(const T& value) {
        std::scoped_lock guard{m_queueMutex};
        m_queue.push(value);
    }

    std::optional<T> dequeue() {
        std::scoped_lock guard{m_queueMutex};
        if (m_queue.empty()) return std::nullopt;

        auto item = m_queue.front();
        m_queue.pop();
        return item;
    }

   private:
    std::queue<T> m_queue;
    mutable std::mutex m_queueMutex;
};
}  // namespace kc::containers
