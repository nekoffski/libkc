#include "ServiceManager.h"

#include <csignal>

namespace kc::service {

static ServiceManager* serviceManagerInstance = nullptr;

static void signalHandler(int signal) {
    ASSERT(serviceManagerInstance != nullptr, "serviceManagerInstance is nullptr");
    LOG_WARN("Received signal {}.", signal);
    serviceManagerInstance->onSignal();
}

ServiceManager::ServiceManager(ServiceThread::Factory* serviceThreadFactory)
    : m_serviceThreadFactory(serviceThreadFactory) {
    serviceManagerInstance = this;
    setupSignals();
}

void ServiceManager::addService(Service* service) {
    m_services.push_back(service);
    LOG_INFO("Service {} added.", service->getName());
}

void ServiceManager::start() {
    initServices();
    startThreads();
    waitForThreads();
    shutdownServices();
}

int ServiceManager::getServicesCount() { return m_services.size(); }

void ServiceManager::setupSignals() {
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);
}

void ServiceManager::onSignal() {
    LOG_INFO("Stopping all running services.");
    for (auto& service : m_services) service->stop();
}

void ServiceManager::initServices() {
    for (auto& service : m_services) {
        auto name = service->getName();

        LOG_INFO("Initializing service {}.", name);
        service->onInit();
        LOG_INFO("Service {} initialized.", name);
    }
}

void ServiceManager::startThreads() {
    m_threads.reserve(m_services.size());

    for (auto& service : m_services) {
        LOG_INFO("Service {} started in thread.", service->getName());
        m_threads.emplace_back(m_serviceThreadFactory->create(service));
    }
}

void ServiceManager::waitForThreads() {
    LOG_INFO("Waiting for services to finish work.");
    for (auto& thread : m_threads) thread->join();
}

void ServiceManager::shutdownServices() {
    for (auto& service : m_services) {
        auto name = service->getName();

        LOG_INFO("Shutting down service {}.", name);
        service->onShutdown();
        LOG_INFO("Service {} shutted down.", name);
    }
}
}  // namespace kc::service