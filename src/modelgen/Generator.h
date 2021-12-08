#pragma once

#include "Core.h"
#include "Language.h"

class Generator {
public:
    struct File {
        std::string content;
        std::string path;
    };

    using Files = std::vector<File>;

    Files generateCode(const Structures& structures);

private:
    std::string generateModel(const Model& model);

    inline static const std::string jsonType = "JSON";

    std::string generateModelFromJson(const Model& model);

    std::string generateModelToJson(const Model& model);

    std::string generateEnum(const Enum& enumerate);

    std::string generateEnumToString(const Enum& enumerate);
};