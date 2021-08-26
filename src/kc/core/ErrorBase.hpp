#pragma once

#include <concepts>
#include <exception>
#include <string>

namespace kc::core {

class ErrorBase : public std::exception {
public:
    explicit ErrorBase(const std::string& details = "No details specified")
        : m_details(details) {
    }

    std::string asString() const {
        return '[' + getName() + "] - " + m_details;
    }

    std::string getDetails() const {
        return m_details;
    }

    virtual std::string getName() const = 0;

private:
    std::string m_details;
};
}

#define DEFINE_SUB_ERROR(ClassName, BaseError)                                  \
    class ClassName : public BaseError {                                        \
    public:                                                                     \
        explicit ClassName(const std::string& details = "No details specified") \
            : BaseError(details) { }                                            \
                                                                                \
        std::string getName() const override {                                  \
            return #ClassName;                                                  \
        }                                                                       \
    };

#define DEFINE_ERROR(ClassName) DEFINE_SUB_ERROR(ClassName, kc::core::ErrorBase)