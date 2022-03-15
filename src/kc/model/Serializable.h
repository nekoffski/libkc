#pragma once

#include <string>

#include "kc/core/Macros.h"
#include "kc/json/Json.h"

namespace kc::model {

struct Serializable {
    virtual std::string getName() const = 0;
    virtual json::Node toJson() const = 0;
    virtual json::Node toMessageJson() const {
        throw json::JsonError{"Not implemented"};
        UNREACHABLE_CODE;
    }
};

}  // namespace kc::model