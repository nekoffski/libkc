#pragma once

#include <string>

#include "kc/json/Json.h"

namespace kc::model {

struct Serializable {
    virtual std::string getName() const = 0;
    virtual json::Node toJson() const = 0;
};

}