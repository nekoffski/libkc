#pragma once

#include <optional>
#include <type_traits>

#include "kc/core/Macros.h"
#include "kc/json/Json.h"

#include "ErrorBase.hpp"
#include "FileSystem.h"

namespace kc::core {

DEFINE_ERROR(ConfigLoaderError);

template <typename T>
requires std::is_default_constructible_v<T> class ConfigLoader {
public:
    ConfigLoader&& fromFile(const std::string& filePath, const FileSystem& fileSystem = FileSystem {}) && {
        checkIfSourceIsNotSpecified();

        if (not fileSystem.isFile(filePath))
            throw ConfigLoaderError { "Could not load file: " + filePath };
        m_configContent = fileSystem.readFile(filePath);

        MOVE_THIS;
    }

    ConfigLoader&& fromString(const std::string& configContent) && {
        checkIfSourceIsNotSpecified();
        m_configContent = configContent;

        MOVE_THIS;
    }

    T&& load() && {
        checkIfSourceIsSpecified();
        loadConfig(m_configContent.value());
        return std::move(m_config);
    }

protected:
    T& config() {
        return m_config;
    }

private:
    virtual void loadConfig(const std::string& configContent) = 0;

    void checkIfSourceIsSpecified() {
        if (not m_configContent.has_value())
            throw ConfigLoaderError { "Source of config content is not specified" };
    }

    void checkIfSourceIsNotSpecified() {
        if (m_configContent.has_value())
            throw ConfigLoaderError { "Source of config content is already specified" };
    }

    T m_config;
    std::optional<std::string> m_configContent;
};

}