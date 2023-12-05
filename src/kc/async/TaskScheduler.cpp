#include "TaskScheduler.hpp"

namespace kc::async {

TaskScheduler::TaskScheduler(int workerCount) :
    m_workerCount(workerCount), m_queues(workerCount), m_currentWorkerId(0l) {
    m_workers.reserve(workerCount);
    for (int i = 0; i < workerCount; ++i)
        m_workers.emplace_back([&, i]() { run(i); });
}

TaskScheduler::~TaskScheduler() {
    for (auto& queue : m_queues) queue.done();
    for (auto& worker : m_workers) worker.join();
}

int TaskScheduler::getWorkerCount() const { return m_workerCount; }

std::optional<TaskScheduler::Task> TaskScheduler::findTask(int workerIndex) {
    // try to steal task if there is nothing for us at this moment
    for (int i = 0; i < m_workerCount; ++i)
        if (auto task = m_queues[(workerIndex + i) % m_workerCount].tryPop(); task)
            return task;
    return m_queues[workerIndex].pop();
}

void TaskScheduler::run(int index) {
    while (true) {
        if (auto task = findTask(index); task) [[likely]]
            std::invoke(task.value());
        else
            break;
    }
}

}  // namespace kc::async
