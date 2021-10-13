#pragma once

#include "Model.hpp"

#include "kc/json/Json.h"

namespace kc::model {

struct Serializable : virtual Model {
    bool isSerializable() const override {
        return true;
    }

    virtual json::Node toJson() const = 0;
};

}