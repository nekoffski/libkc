#pragma once

#include <jsoncpp/json/json.h>

#include <sstream>

#include "kc/core/ErrorBase.hpp"

using namespace std::literals::string_literals;

namespace kc::json {

DEFINE_ERROR(JsonError);
DEFINE_SUB_ERROR(CouldNotLoadJson, JsonError);
DEFINE_SUB_ERROR(CouldNotCreateJson, JsonError);

using Node = Json::Value;

Json::Value loadJson(const std::string&);

std::string toString(const Node& node);

class JsonBuilder {
   public:
    explicit JsonBuilder();

    std::string asString();
    Json::Value asJsonObject();

    JsonBuilder& beginObject();
    JsonBuilder& beginObject(const std::string& name);
    JsonBuilder& endObject();
    JsonBuilder& beginArray();
    JsonBuilder& beginArray(const std::string& name);
    JsonBuilder& endArray();

    JsonBuilder& addField(const std::string& value) { return addFieldImpl('"' + value + '"'); }

    JsonBuilder& addField(const std::string& key, const std::string& value) {
        return addFieldImpl(key, '"' + value + '"');
    }

    template <typename T> JsonBuilder& addField(const T& item) { return addFieldImpl(item); }

    template <typename T> JsonBuilder& addField(const std::string& key, const T& value) {
        return addFieldImpl(key, value);
    }

    template <typename T, template <typename> typename C>
    JsonBuilder& addField(const std::string& key, const C<T>& values) {
        beginArray(key);
        for (const auto& value : values) addField(value);
        return endArray();
    }

   private:
    template <typename T> JsonBuilder& addFieldImpl(const T& value) {
        if (m_shouldInsertComma) m_jsonStream << ',';

        m_jsonStream << value;
        m_shouldInsertComma = true;
        return *this;
    }

    template <typename T> JsonBuilder& addFieldImpl(const std::string& key, const T& value) {
        if (m_shouldInsertComma) m_jsonStream << ',';
        m_jsonStream << '"' << key << "\": " << value;
        m_shouldInsertComma = true;
        return *this;
    }

    void reset();

    std::stringstream m_jsonStream;
    bool m_shouldInsertComma;
};
}  // namespace kc::json
