#include "ThreadPool.hpp"

#include <chrono>

#include "kc/core/Macros.h"

namespace kc::async {

ThreadPool::ThreadPool(const std::size_t size)
    : m_active(true)
    , m_size(size) {

    m_threads.reserve(size);

    repeat(size)
        m_threads.emplace_back(&ThreadPool::threadWorker, this);
}

ThreadPool::~ThreadPool() {
    stop();
}

std::size_t ThreadPool::getSize() const {
    return m_size;
}

void ThreadPool::stop() {
    m_active = false;
}

void ThreadPool::threadWorker() {
    using namespace std::chrono_literals;

    static const auto sleepPeriod = 100ms;

    // TODO: looks like busy-wait, consider using condition variable + mutex pattern
    while (m_active) {
        auto job = m_jobs.dequeue();

        if (not job.has_value()) {
            std::this_thread::sleep_for(sleepPeriod);
            continue;
        }

        std::invoke(*job);
    }
}

}
