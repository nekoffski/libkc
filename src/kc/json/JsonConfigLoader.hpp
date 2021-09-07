#pragma once

#include "kc/core/ConfigLoader.hpp"

namespace kc::json {

template <typename T>
requires std::is_default_constructible_v<T> class JsonConfigLoader : public core::ConfigLoader<T> {
private:
    virtual void processConfigFields(const Node& root) = 0;

    void loadConfig(const std::string& configContent) override {
        processConfigFields(loadJson(configContent));
    }
};

}