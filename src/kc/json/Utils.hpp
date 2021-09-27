#pragma once

#include "Json.h"
#include "kc/core/ErrorBase.hpp"
#include "kc/core/Macros.h"
#include "kc/core/Meta.hpp"

namespace kc::json {

DEFINE_SUB_ERROR(JsonLogicError, JsonError);

namespace detail {
    using namespace core;

    template <typename Error>
    requires std::derived_from<Error, core::ErrorBase> void kc_assert(bool condition, const std::string& errorMessage) {
        if (not condition)
            throw Error { errorMessage };
    }

    template <typename Error, typename T>
    enable_for(float) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        kc_assert<Error>(node.isDouble(), messagePrefix + " has wrong type");
        return node.asFloat();
    }

    template <typename Error, typename T>
    enable_for(int) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        kc_assert<Error>(node.isInt(), messagePrefix + " has wrong type");
        return node.asInt();
    }

    template <typename Error, typename T>
    enable_for(std::string) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        kc_assert<Error>(node.isString(), messagePrefix + " has wrong type");
        return node.asString();
    }

    template <typename Error, typename T>
    enable_for(unsigned int) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        kc_assert<Error>(node.isUInt(), messagePrefix + " has wrong type");
        return node.asUInt();
    }

    template <typename Error, typename T>
    enable_for(bool) tryToExtractValue(const Node& node, const std::string& messagePrefix) {
        kc_assert<Error>(node.isBool(), messagePrefix + " has wrong type");
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
            kc_assert<Error>(this->m_value >= minValue, "Value of field " + this->m_fieldName + "is ");
            MOVE_THIS;
        }

        ValueWrapper&& max(T maxValue) && {
            kc_assert<Error>(this->m_value <= maxValue, "");
            MOVE_THIS;
        }

        ValueWrapper&& inRange(T minValue, T maxValue) && {
            MOVE_THIS.min(minValue).max(maxValue);
        }
    };

    template <typename Error, typename T>
    class ValueWrapper<Error, T, typename std::enable_if_t<is_one_of2_v<T, std::string, Node>>>
        : public ValueWrapperBase<Error, T> {

    public:
        using ValueWrapperBase<Error, T>::ValueWrapperBase;

        ValueWrapper&& nonEmpty() && {
            kc_assert<Error>(not this->m_value.empty(), "");
            MOVE_THIS;
        }

        ValueWrapper&& minSize(std::size_t min) && {
            kc_assert<Error>(this->m_value.size() >= min, "");
            MOVE_THIS;
        }

        ValueWrapper&& maxSize(std::size_t max) && {
            kc_assert<Error>(this->m_value.size() <= max, "");
            MOVE_THIS;
        }

        ValueWrapper&& sizeInRange(std::size_t min, std::size_t max) && {
            MOVE_THIS.minSize(min).maxSize(max);
        }
    };

    template <typename Error>
    struct ArrayWrapper : public ValueWrapper<Error, Node> {
        using ValueWrapper<Error, Node>::ValueWrapper;

        template <typename U>
        ArrayWrapper&& ofType() && {
            for (auto& value : this->m_value)
                tryToExtractValue<Error, U>(value, "Field of array " + this->m_fieldName);
            MOVE_THIS;
        }
    };

    template <typename Error>
    class NodeWrapper {
    public:
        explicit NodeWrapper(const Node& node)
            : m_node(node) {
        }

        NodeWrapper&& ofName(const std::string& name) && {
            kc_assert<JsonLogicError>(not m_name.has_value(), "Name of field is already specified");
            kc_assert<Error>(m_node.isMember(name), "Could not found field with name: " + name);
            m_name = name;

            MOVE_THIS;
        }

        template <typename T>
        ValueWrapper<Error, T> ofType() && {
            kc_assert<JsonLogicError>(m_name.has_value(), "Field has not specified its name");
            auto kc_assertMessagePrefix = "Field with name: " + m_name.value();

            return ValueWrapper<Error, T> {
                tryToExtractValue<Error, T>(m_node.get(m_name.value(), {}), kc_assertMessagePrefix), m_name.value()
            };
        }

        ArrayWrapper<Error> asArray() {
            kc_assert<JsonLogicError>(m_name.has_value(), "Field has not specified its name");

            auto child = m_node.get(m_name.value(), {});
            kc_assert<Error>(child.isArray(), "Field with name " + m_name.value() + " has invalid type");

            return ArrayWrapper<Error> { child, m_name.value() };
        }

        ValueWrapper<Error, Node> asObject() {
            kc_assert<JsonLogicError>(m_name.has_value(), "Field has not specified its name");

            auto child = m_node.get(m_name.value(), {});
            kc_assert<Error>(child.isObject(), "Field with name " + m_name.value() + " has invalid type");

            return ValueWrapper<Error, Node> { child, m_name.value() };
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
