#include "Generator.h"

#include <iostream>
#include <ranges>
#include <sstream>

#include "Core.h"

Generator::Generator(JsonLib jsonLib)
    : m_jsonLib(jsonLib) {
}

Generator::Files Generator::generateCode(const Structures& structures) {
    Files files;

    for (auto& enumerate : structures.enums) {
        files.emplace_back(
            generateEnum(enumerate), enumerate.name + ".hpp");
        m_customTypes[enumerate.name] = Type::enumerate;
    }

    for (auto& model : structures.models)
        m_customTypes[model.name] = Type::model;

    for (auto& model : structures.models)
        files.emplace_back(
            generateModel(model), model.name + ".hpp");

    return files;
}

std::string Generator::determineFieldType(const std::string& typeDescription, std::ostringstream& headers) {
    if (Model::allowedTypes.contains(typeDescription))
        return Model::allowedTypes.at(typeDescription);

    if (m_customTypes.contains(typeDescription)) {
        if (m_customTypes[typeDescription] == Type::model)
            headers << "#include \"" << typeDescription << ".h\"\n";

        return typeDescription;
    }

    throw ModelGeneratorError { "Unknown type: " + typeDescription };
}

std::string Generator::generateModel(const Model& model) {
    std::ostringstream data;
    std::ostringstream headers;

    headers << "#pragma once\n\n"
            << "#include \"kc/model/Model.h\"\n";

    data << "struct " << model.name << " : public Model<" << model.name << '>';

    if (m_jsonLib == JsonLib::libkc)
        data << ", protected kc::json::NodeHelper<DeserializationError> ";

    data << "{\n\n";

    for (auto& [fieldName, fieldType] : model.fields)
        data << spaces(4) << determineFieldType(fieldType, headers) << ' ' << fieldName << ";\n";

    data << '\n'
         << spaces(4) << "std::string getName() const override {\n"
         << spaces(8) << "return " << '\"' << model.name << "\";\n"
         << spaces(4) << "}\n";

    data << '\n'
         << generateModelFromJson(model) << '\n'
         << generateModelToJson(model)
         << "};\n";

    return headers.str() + "\n" + data.str();
}

std::string Generator::generateModelFromJson(const Model& model) {
    std::ostringstream data;

    data << spaces(4) << "static " << model.name << " fromJson(const " << getJsonObjectType() << "& json) {\n"
         << spaces(8) << "return " << model.name << " {\n";

    static auto generateGetter = [](const Model::Field& field) -> std::string {
        const auto& [name, type] = field;

        if (Model::allowedTypes.contains(type))
            return "fieldFrom(json).withName(\"" + Model::allowedTypes.at(type) + "\").ofType<" + "int" + ">().get(),\n";

        return type + "::fromJson(fieldFrom(json).withName(\"" + name + "\").asObject().get()),\n";
    };

    for (const auto& field : model.fields)
        data << spaces(12) << '.' << field.name << " = " << generateGetter(field);

    data << spaces(8) << "};\n"
         << spaces(4) << "}\n";

    return data.str();
}

std::string Generator::generateModelToJson(const Model& model) {
    std::ostringstream data;

    if (m_jsonLib == JsonLib::libkc) {

        data << spaces(4) << getJsonObjectType() << " toJson() const override {\n"
             << spaces(8) << "kc::core::JsonBuilder json;"
             << "\n\n";

        data << spaces(8) << "json.addField(\"name\", getName()).beginObject(\"body\");\n\n";

        for (const auto& [name, type] : model.fields) {
            data << spaces(8) << "json.addField(\"" << name << "\", "
                 << "this->" << name;

            std::cout << "Processing field: " << type << "/" << name << '\n';

            if (not Model::allowedTypes.contains(type))
                data << ".toJson()";

            data << ");\n";
        }

        data << '\n'
             << spaces(8) << "json.endObject();\n";

        data << '\n'
             << spaces(8) << "return json.asString();\n"
             << spaces(4) << "}\n";
    } else {
        throw ModelGeneratorError { "Arduino JSON is not supported yet" };
    }

    return data.str();
}

std::string Generator::generateEnum(const Enum& enumerate) {
    std::ostringstream data;

    static const std::string enumType = "unsigned char";

    data << "#pragma once\n\n";
    data << "enum " << enumerate.name << " : " << enumType << " {\n";
    data << spaces(4);

    for (auto& value : enumerate.values)
        data << value << ", ";

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
