#pragma once

#include <memory>

#include <boost/system/error_code.hpp>

namespace kc::async {

class Error {
public:
    static Error noError() {
        return Error {};
    }

    explicit Error(boost::system::error_code ec)
        : m_hasValue(static_cast<bool>(ec)) {

        if (m_hasValue)
            m_details = ec.message();
    }

    explicit Error(const std::string& details)
        : m_hasValue(true)
        , m_details(details) {
    }

    std::string asString() const {
        return m_details;
    }

    operator bool() const {
        return m_hasValue;
    }

private:
    Error()
        : m_hasValue(false) {
    }

    bool m_hasValue;
    std::string m_details;
};

}