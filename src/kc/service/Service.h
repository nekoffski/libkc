#pragma once

#include <string>

#include "ServiceBase.h"

namespace kc::service {

class Service : public ServiceBase {
   public:
    Service(const std::string& name);

    void run() final override;

   private:
    bool m_isRunning;
};
}  // namespace kc::service