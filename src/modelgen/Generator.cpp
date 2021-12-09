#include "Generator.h"

#include <sstream>

#include "Core.h"

Generator::Generator(JsonLib jsonLib)
    : m_jsonLib(jsonLib) {
}

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
         << "#include \"BaseModel.h\"\n\n"
         << "struct " << model.name << " : BaseModel";

    if (m_jsonLib == JsonLib::libkc)
        data << ", protected kc::json::NodeHelper<DeserializationError> ";

    data << "{\n\n";

    for (auto& [fieldName, fieldType] : model.fields)
        data << spaces(4) << fieldType << ' ' << fieldName << ";\n";

    data << '\n'
         << spaces(4) << "std::string getName() const override {\n"
         << spaces(8) << "return " << '\"' << model.name << "\";\n"
         << spaces(4) << "}\n";

    data << '\n'
         << generateModelFromJson(model) << '\n'
         << generateModelToJson(model)
         << "};\n";

    return data.str();
}

std::string Generator::generateModelFromJson(const Model& model) {
    std::ostringstream data;

    data << spaces(4) << "static " << model.name << " fromJson(const " << getJsonObjectType() << "& json) {\n"
         << spaces(8) << "return " << model.name << " {\n";

    static auto generateGetter = [](const Model::Field& field) -> std::string {
        const auto& [name, type] = field;

        return "fieldFrom(json).withName(\"" + name + "\").ofType<" + Model::allowedTypes[type] + ">().get(),\n";
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

        for (const auto& [name, type] : model.fields) {
            data << spaces(8) << "json.addField(\"" << name << "\", "
                 << "this->" << name;

            if (not Model::allowedTypes.contains(type))
                data << ".asString()";

            data << ");\n";
        }

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