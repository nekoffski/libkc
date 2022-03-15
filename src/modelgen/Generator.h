#pragma once

#include "Core.h"
#include "Language.h"
#include "kc/core/FileSystem.h"

class Generator {
   public:
    struct File {
        std::string content;
        std::string path;
    };

    using Files = std::vector<File>;

    explicit Generator(JsonLib jsonLib);

    Files generateCode(const Structures& structures);

   private:
    std::string generateModel(const Model& model);

    inline static const std::string jsonType = "JSON";

    inline std::string getJsonObjectType() const {
        return m_jsonLib == JsonLib::libkc ? "kc::json::Node" : "JSON";
    }

    std::string generateConstructor(const Model& model);

    std::string generateModelFromJson(const Model& model);

    std::string generateModelToJson(const Model& model);

    std::string generateEnum(const Enum& enumerate);

    std::string generateEnumToString(const Enum& enumerate);

    std::string determineFieldType(const std::string& fieldDescription,
                                   std::ostringstream* headers = nullptr);

    JsonLib m_jsonLib;

    enum Type { enumerate, model };

    std::unordered_map<std::string, Type> m_customTypes;
};

void generateFiles(kc::core::FileSystem& fs, const Generator::Files& files, OutputType outputType);