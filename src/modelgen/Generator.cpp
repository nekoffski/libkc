#include "Generator.h"

#include <sstream>

#include "Core.h"

Generator::Files Generator::generateCode(const Structures& structures) {
    Files files;

    for (auto& enumerate : structures.enums)
        files.emplace_back(
            generateEnum(enumerate), enumerate.name + ".hpp");

    for (auto& model : structures.models)
        files.emplace_back(
            generateModel(model), model.name + ".hpp");

    return files;
}

std::string Generator::generateModel(const Model& model) {
    std::ostringstream data;

    data << "#pragma once\n\n"
         << "struct " << model.name << " {\n";

    for (auto& [fieldName, fieldType] : model.fields)
        data << fourSpaces << fieldType << ' ' << fieldName << ";\n";

    data << '\n'
         << generateModelFromJson(model) << '\n'
         << generateModelToJson(model) << '\n'
         << "};\n";

    return data.str();
}

inline static const std::string jsonType = "JSON";

std::string Generator::generateModelFromJson(const Model& model) {
    std::ostringstream data;

    data << fourSpaces << "static " << model.name << " fromJson(const " << jsonType << "& json) {\n"
         << fourSpaces << fourSpaces << "return "
         << "JSON {};\n"
         << fourSpaces << "}\n";

    return data.str();
}

std::string Generator::generateModelToJson(const Model& model) {
    std::ostringstream data;

    data << fourSpaces << jsonType << " toJson(const " << model.name << "& model) {\n";
    data << fourSpaces << fourSpaces << "return " << model.name << " {};\n";
    data << fourSpaces << "}\n";

    return data.str();
}

std::string Generator::generateEnum(const Enum& enumerate) {
    std::ostringstream data;

    static const std::string enumType = "unsigned char";

    data << "#pragma once\n\n";
    data << "enum " << enumerate.name << " : " << enumType << " {\n";
    data << fourSpaces;

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
    data << fourSpaces << "switch (value) {\n";

    for (auto& value : enumerate.values) {
        data << fourSpaces << fourSpaces << "case " << enumerate.name << "::" << value << ":\n";
        data << fourSpaces << fourSpaces << fourSpaces << "return "
             << "\"" << value << "\";\n";
    }

    data << fourSpaces << "}\n";
    data << fourSpaces << "return \"UnknownValue\";\n";
    data << "}\n";

    data << "\n";

    return data.str();
}