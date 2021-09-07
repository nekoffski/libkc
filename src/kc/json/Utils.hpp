#pragma once

#include "Json.h"
#include "kc/core/ErrorBase.hpp"
#include "kc/core/Macros.h"

namespace kc::json {

DEFINE_SUB_ERROR(JsonLogicError, JsonError);

namespace detail {
    template <typename Error>
    requires std::derived_from<Error, core::ErrorBase> void assert(bool condition, const std::string& errorMessage) {
        if (not condition)
            throw Error { errorMessage };
    }

    template <typename T, typename U>
    using enable_for_impl = std::enable_if<std::is_same<T, U>::value, U>;

    // clang-format off
    #define enable_for(_type) typename enable_for_impl<T, _type>::type
    // clang-format on

    template <typename Error, typename T>
    enable_for(float) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        assert<Error>(node.isDouble(), messagePrefix + " has wrong type");
        return node.asFloat();
    }

    template <typename Error, typename T>
    enable_for(int) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        assert<Error>(node.isInt(), messagePrefix + " has wrong type");
        return node.asInt();
    }

    template <typename Error, typename T>
    enable_for(std::string) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        assert<Error>(node.isString(), messagePrefix + " has wrong type");
        return node.asString();
    }

    template <typename Error, typename T>
    enable_for(unsigned int) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        assert<Error>(node.isUInt(), messagePrefix + " has wrong type");
        return node.asUInt();
    }

    template <typename Error, typename T>
    enable_for(bool) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        assert<Error>(node.isBool(), messagePrefix + " has wrong type");
        return node.asBool();
    }

    template <typename Error, typename T>
    requires std::is_default_constructible_v<T> class ValueWrapperBase {
    public:
        explicit ValueWrapperBase(const T& value, const std::string& fieldName)
            : m_value(value)
            , m_fieldName(fieldName) {
        }

        T&& get() && {
            return std::move(m_value);
        }

    protected:
        T m_value;
        const std::string& m_fieldName;
    };

    template <typename Error, typename T, typename Enabled = void>
    class ValueWrapper {
    };

    template <typename Error, typename T>
    struct ValueWrapper<Error, T, typename std::enable_if_t<std::is_arithmetic_v<T>>> : public ValueWrapperBase<Error, T> {
        using ValueWrapperBase<Error, T>::ValueWrapperBase;

        ValueWrapper&& min(T minValue) && {
            assert<Error>(this->m_value >= minValue, "Value of field " + this->m_fieldName + "is ");
            MOVE_THIS;
        }

        ValueWrapper&& max(T maxValue) && {
            assert<Error>(this->m_value <= maxValue, "");
            MOVE_THIS;
        }

        ValueWrapper&& inRange(T minValue, T maxValue) && {
            MOVE_THIS.min(minValue).max(maxValue);
        }
    };

    template <typename Error>
    struct ValueWrapper<Error, std::string> : public ValueWrapperBase<Error, std::string> {
        using ValueWrapperBase<Error, std::string>::ValueWrapperBase;

        ValueWrapper&& nonEmpty() && {
            assert<Error>(not this->m_value.empty(), "");
            MOVE_THIS;
        }

        ValueWrapper&& minLength(std::size_t min) && {
            assert<Error>(this->m_value.size() >= min, "");
            MOVE_THIS;
        }

        ValueWrapper&& maxLength(std::size_t max) && {
            assert<Error>(this->m_value.size() <= max, "");
            MOVE_THIS;
        }

        ValueWrapper&& lengthInRange(std::size_t min, std::size_t max) && {
            MOVE_THIS.minLength(min).maxLength(max);
        }
    };

    template <typename Error>
    class NodeWrapper {
    public:
        explicit NodeWrapper(const Node& node)
            : m_node(node) {
        }

        NodeWrapper&& ofName(const std::string& name) && {
            assert<JsonLogicError>(not m_name.has_value(), "Name of field is already specified");
            assert<Error>(m_node.isMember(name), "Could not found field with name: " + name);
            m_name = name;

            MOVE_THIS;
        }

        template <typename T>
        ValueWrapper<Error, T> ofType() && {
            assert<JsonLogicError>(m_name.has_value(), "Field has not specified its name");
            auto assertMessagePrefix = "Field with name: " + m_name.value();

            return ValueWrapper<Error, T> {
                tryToExtractValue<Error, T>(m_node.get(m_name.value(), {}), assertMessagePrefix), m_name.value()
            };
        }

        void asArray() {
        }

        void asObject() {
        }

    private:
        const Node& m_node;
        std::optional<std::string> m_name;
    };

}

template <typename Error>
requires std::derived_from<Error, core::ErrorBase> class NodeHelper {
protected:
    detail::NodeWrapper<Error> fieldFrom(const Node& node) {
        return detail::NodeWrapper<Error> { node };
    }
};

}
