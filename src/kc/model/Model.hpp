#pragma once

#include <string>

namespace kc::model {

struct Model {
    virtual bool isSerializable() const {
        return false;
    }

    virtual bool isDeserializable() const {
        return false;
    }

    virtual const std::string& getName() const = 0;
};

}