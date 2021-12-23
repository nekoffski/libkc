#pragma once

#include <memory>
#include <string>

#include "Deserializable.h"
#include "kc/json/Json.h"

namespace kc::model {

struct MessageDeserializer {
    virtual std::unique_ptr<Deserializable> deserializeMessage(
        const std::string& header, const json::Node& body) const = 0;
};

}