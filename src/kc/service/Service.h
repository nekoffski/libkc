#pragma once

#include <chrono>
#include <string>
#include <thread>

namespace kc::service {

class Service {
   public:
    Service(const std::string& name);

    virtual ~Service() = default;

    virtual void update() = 0;
    virtual void onInit() = 0;
    virtual void onShutdown() = 0;

    void run();
    void stop();

    std::string getName() const;

   protected:
    std::string m_name;

   private:
    bool m_isRunning;
};
}  // namespace kc::service