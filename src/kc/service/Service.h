#pragma once

#include <chrono>
#include <string>
#include <thread>

#include "kc/core/Log.h"

namespace kc::service {

class Service {
public:
    Service(const std::string& name)
        : m_name(name)
        , m_isRunning(true) {
    }

    virtual ~Service() = default;

    virtual void update() = 0;
    virtual void onInit() = 0;
    virtual void onShutdown() = 0;

    void run() {
        using namespace std::chrono_literals;

        while (m_isRunning) {
            update();
            std::this_thread::sleep_for(50ms);
        }
    }

    void stop() {
        LOG_INFO("Forcing service {} to stop.", m_name);
        m_isRunning = false;
    }

    std::string getName() const {
        return m_name;
    }

protected:
    std::string m_name;

private:
    bool m_isRunning;
};
}