#include "Generator.h"

#include <iostream>
#include <ranges>
#include <sstream>

#include "Core.h"
#include "kc/core/Enumerate.hpp"

Generator::Generator(JsonLib jsonLib) : m_jsonLib(jsonLib) {}

Generator::Files Generator::generateCode(const Structures& structures) {
    Files files;

    for (auto& enumerate : structures.enums) {
        std::cout << "$ Generating enum: " << enumerate.name << '\n';
        files.emplace_back(generateEnum(enumerate), enumerate.name + ".hpp");
        m_customTypes[enumerate.name] = Type::enumerate;
    }

    for (auto& model : structures.models) m_customTypes[model.name] = Type::model;

    for (auto& model : structures.models) {
        std::cout << "$ Generating model: " << model.name << '\n';
        files.emplace_back(generateModel(model), model.name + ".hpp");
    }

    return files;
}

std::string Generator::determineFieldType(const std::string& typeDescription,
                                          std::ostringstream* headers) {
    if (Model::allowedTypes.contains(typeDescription))
        return Model::allowedTypes.at(typeDescription);

    if (typeDescription[0] == '[') {
        auto type = typeDescription.substr(1, typeDescription.length() - 2);

        if (headers != nullptr && m_customTypes.contains(type))
            if (m_customTypes[type] == Type::model) *headers << "#include \"" << type << ".hpp\"\n";

        return "std::vector<" + type + ">";
    }

    if (headers != nullptr && m_customTypes.contains(typeDescription)) {
        if (m_customTypes[typeDescription] == Type::model)
            *headers << "#include \"" << typeDescription << ".hpp\"\n";

        return typeDescription;
    }

    throw ModelGeneratorError{"Unknown type: " + typeDescription};
}

std::string Generator::generateModel(const Model& model) {
    std::ostringstream data;
    std::ostringstream headers;

    headers << "#pragma once\n\n"
            << "#include \"kc/model/Model.h\"\n"
            << "#include \"kc/json/Utils.hpp\"\n"
            // TODO: this should happen on a condition
            << "#include \"kc/event/Event.h\"\n\n";

    data << "struct " << model.name << " : public kc::model::Model<" << model.name << ">";

    if (model.isMessage)
        data << ",\n" << spaces(8) << "public kc::event::EventBase<" << model.name << "> ";
    else
        data << ' ';

    data << "{\n";

    data << generateConstructor(model);

    for (auto& [fieldName, fieldType] : model.fields) {
        std::cout << fieldName << '/' << fieldType << '\n';
        data << spaces(4) << determineFieldType(fieldType, &headers) << ' ' << fieldName << ";\n";
    }

    data << '\n'
         << spaces(4) << "std::string getName() const override {\n"
         << spaces(8) << "return " << '\"' << model.name << "\";\n"
         << spaces(4) << "}\n";

    data << '\n' << generateModelFromJson(model) << '\n' << generateModelToJson(model) << "};\n";

    return headers.str() + "\n" + data.str();
}

std::string Generator::generateConstructor(const Model& model) {
    std::ostringstream data;

    data << spaces(4) << "explicit " << model.name << "() = default;\n";

    if (const auto modelFields = model.fields.size(); modelFields > 0) {
        data << spaces(4) << "explicit " << model.name << "(\n";

        for (int i = 0; i < modelFields; ++i) {
            const auto& [fieldName, fieldType] = model.fields[i];

            data << spaces(8) << determineFieldType(fieldType) << ' ' << fieldName;

            if (i < modelFields - 1) data << ", \n";
        }

        data << '\n' << spaces(4) << ") : \n";

        for (int i = 0; i < modelFields; ++i) {
            const auto& name = model.fields[i].name;

            data << spaces(8) << name << '(' << name << ")";

            if (i < modelFields - 1) data << ", \n";
        }

        data << '\n' << spaces(4) << "{}\n\n";
    }

    return data.str();
}

std::string Generator::generateModelFromJson(const Model& model) {
    std::ostringstream data;

    data << spaces(4) << "static " << model.name << " fromJson(const " << getJsonObjectType()
         << "& json) {\n"
         << spaces(8) << model.name << " model;\n\n";

    static auto generateGetter = [](const Model::Field& field) -> std::string {
        const auto& [name, type] = field;

        if (Model::allowedTypes.contains(type))
            return "kc::json::fieldFrom<ModelError>(json).withName(\"" + name + "\").ofType<" +
                   Model::allowedTypes.at(type) + ">().get();\n";

        return type + "::fromJson(kc::json::fieldFrom<ModelError>(json).withName(\"" + name +
               "\").asObject().get());\n";
    };

    for (const auto& field : model.fields) {
        if (field.type[0] == '[') {
            auto fieldType = field.type.substr(1, field.type.length() - 2);

            data << spaces(8)
                 << "for (auto& node : kc::json::fieldFrom<ModelError>(json).withName(\""
                 << field.name << "\").asArray().get())\n"
                 << spaces(12) << "model." << field.name << ".push_back(" << fieldType
                 << "::fromJson(node));\n";

        } else {
            data << spaces(8) << "model." << field.name << " = " << generateGetter(field);
        }
    }
    data << spaces(8) << '\n' << spaces(8) << "return model;\n" << spaces(4) << "}\n";

    return data.str();
}

std::string Generator::generateModelToJson(const Model& model) {
    std::ostringstream data;

    if (m_jsonLib == JsonLib::libkc) {
        data << spaces(4) << getJsonObjectType() << " toJson() const override {\n"
             << spaces(8) << "kc::json::JsonBuilder json;"
             << "\n\n";

        for (const auto& [name, type] : model.fields) {
            std::cout << "Processing field: " << type << "/" << name << '\n';

            if (type[0] == '[') {
                data << spaces(8) << "json.beginArray(\"" << name << "\");\n"
                     << spaces(8) << "for (const auto& item : this->" << name << ")\n"
                     << spaces(12) << "json.addField(item";

                if (not Model::allowedTypes.contains(type.substr(1, type.length() - 2)))
                    data << ".toJson()";

                data << ");\n" << spaces(8) << "json.endArray();\n";

            } else {
                data << spaces(8) << "json.addField(\"" << name << "\", "
                     << "this->" << name;

                if (not Model::allowedTypes.contains(type)) data << ".toJson()";

                data << ");\n";
            }
        }

        data << '\n' << spaces(8) << "return json.asJsonObject();\n" << spaces(4) << "}\n";

        if (model.isMessage) {
            data << '\n'
                 << spaces(4) << getJsonObjectType() << " toMessageJson() const override {\n"
                 << spaces(8) << "kc::json::JsonBuilder json;"
                 << "\n\n";
            data << spaces(8)
                 << "json.addField(\"name\", getName()).addField(\"body\", toJson());\n";
            data << '\n' << spaces(8) << "return json.asJsonObject();\n" << spaces(4) << "}\n";
        }
    } else {
        throw ModelGeneratorError{"Arduino JSON is not supported yet"};
    }

    return data.str();
}

std::string Generator::generateEnum(const Enum& enumerate) {
    std::ostringstream data;

    static const std::string enumType = "unsigned char";

    data << "#pragma once\n\n";
    data << "enum " << enumerate.name << " : " << enumType << " {\n";
    data << spaces(4);

    for (auto& value : enumerate.values) data << value << ", ";

    data << '\n';
    data << "};\n\n";
    data << generateEnumToString(enumerate) << "\n";

    return data.str();
}

std::string Generator::generateEnumToString(const Enum& enumerate) {
    std::ostringstream data;

    data << "const std::string toString(" << enumerate.name << " value) {\n";
    data << spaces(4) << "switch (value) {\n";

    for (auto& value : enumerate.values) {
        data << spaces(8) << "case " << enumerate.name << "::" << value << ":\n";
        data << spaces(12) << "return "
             << "\"" << value << "\";\n";
    }

    data << spaces(4) << "}\n";
    data << spaces(4) << "return \"UnknownValue\";\n";
    data << "}\n";

    data << "\n";

    return data.str();
}

void generateFiles(kc::core::FileSystem& fs, const Generator::Files& files, OutputType outputType) {
}
