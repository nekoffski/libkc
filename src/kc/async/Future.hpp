#pragma once

#include <chrono>
#include <future>
#include <vector>

namespace kc::async {

template <typename T>
class Future {
public:
    explicit Future(std::future<T>&& future)
        : m_future(std::move(future)) {
    }

    bool isReady() {
        return m_future.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
    }

    T getValue() {
        return m_future.get();
    }

    void wait() const {
        m_future.wait();
    }

private:
    std::future<T> m_future;
};

template <typename... Args>
void wait(Args&&... args) {
    (wait(args), ...);
}

template <typename T>
void wait(Future<T>& future) {
    future.wait();
}

template <typename T>
void wait(const std::vector<Future<T>>& futures) {
    for (auto& future : futures)
        future.wait();
}
}
