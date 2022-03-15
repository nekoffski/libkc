#pragma once

#include <string>

#include "kc/core/Macros.h"

namespace kc::service {

class ServiceBase {
   public:
    explicit ServiceBase(const std::string& name) : m_name(name) {}

    virtual ~ServiceBase() = default;

    virtual void call() = 0;

    virtual void run() = 0;
    virtual void stop() = 0;

    virtual void onInit() EMPTY_METHOD;
    virtual void onShutdown() EMPTY_METHOD;

    std::string getName() const { return m_name; }

   protected:
    std::string m_name;
};

}  // namespace kc::service