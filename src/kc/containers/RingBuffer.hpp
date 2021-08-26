#pragma once

#include <mutex>
#include <utility>
#include <vector>

namespace kc::containers {

template <typename T>
class RingBuffer {
    using Size = std::size_t;

public:
    constexpr inline static Size defaultCapacity = 1000;

    explicit RingBuffer(const Size capacity = defaultCapacity)
        : m_pushIndex(0)
        , m_popIndex(0)
        , m_capacity(capacity) {

        m_buffer.resize(m_capacity);
    }

    void push(const T& value) {
        std::scoped_lock guard { m_pushMutex };

        auto index = std::exchange(m_pushIndex, (m_pushIndex + 1) % m_capacity);
        m_buffer[index] = value;
    }

    void push(T&& value) {
        std::scoped_lock guard { m_pushMutex };

        auto index = std::exchange(m_pushIndex, (m_pushIndex + 1) % m_capacity);
        m_buffer[index] = std::move(value);
    }

    [[nodiscard]] T&& pop() {
        std::scoped_lock guard { m_popMutex };
        auto index = std::exchange(m_popIndex, (m_popIndex + 1) % m_capacity);
        return std::move(m_buffer[index]);
    }

    Size getCapacity() const {
        return m_capacity;
    }

    bool isEmpty() const {
        std::scoped_lock guard { m_pushMutex, m_popMutex };
        return m_pushIndex == m_popIndex;
    }

private:
    std::vector<T> m_buffer;

    Size m_pushIndex;
    Size m_popIndex;

    const Size m_capacity;

    mutable std::mutex m_pushMutex;
    mutable std::mutex m_popMutex;
};

}