#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Model {
    struct Field {
        std::string name;
        std::string type;
    };

    inline static const std::unordered_map<std::string, std::string> allowedTypes = {
        {"int",     "int"        },
        {"bool",    "bool"       },
        {"str",     "std::string"},
        {"numeric", "float"      }
    };

    bool isMessage;
    std::string name;
    std::vector<Field> fields;
};

struct Enum {
    std::string name;
    std::vector<std::string> values;
};

struct Structures {
    std::vector<Enum> enums;
    std::vector<Model> models;
};