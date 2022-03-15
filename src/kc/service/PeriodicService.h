#pragma once

#include <string>

#include "ServiceBase.h"

namespace kc::service {

class PeriodicService : public ServiceBase {
   public:
    PeriodicService(const std::string& name);

    void run() final override;
    void stop() final override;

   private:
    bool m_isRunning;
};
}  // namespace kc::service