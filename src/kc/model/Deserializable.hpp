#pragma once

#include "Model.hpp"

namespace kc::model {

struct Deserializable : virtual Model {
    bool isDeserializable() const override {
        return true;
    }
};

}