#include <iostream>

#include <stack>

#include <ranges>
#include <unordered_map>

#include <kc/core/ErrorBase.hpp>
#include <kc/core/FileSystem.h>
#include <kc/core/Log.h>

DEFINE_ERROR(ModelGeneratorError);
DEFINE_SUB_ERROR(SyntaxError, ModelGeneratorError);

using namespace kc;

enum class OutputType {
    singleFile,
    multiFile,
};

auto pop(auto stack) {
    auto tmp = stack.top();
    stack.pop();

    return tmp;
}

enum class TokenType : unsigned char {
    header,
    string,
    colon,
    openingBrace,
    closingBrace,
    coma,
    constraint
};

std::string toString(TokenType tokenType) {
    switch (tokenType) {
    case TokenType::header:
        return "[Header]";
    case TokenType::string:
        return "[String]";
    case TokenType::colon:
        return "[Colon]";
    case TokenType::openingBrace:
        return "[OpeningBrace]";
    case TokenType::closingBrace:
        return "[ClosingBrace]";
    case TokenType::coma:
        return "[Coma]";
    case TokenType::constraint:
        return "[Constraint]";
    }
    return "UnknownToken";
}

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer {
public:
    using Tokens = std::vector<Token>;

    Tokens tokenize(const std::vector<std::string>& file) {
        Tokens tokens;

        static auto isLineNotEmpty = [](const std::string& line) -> bool {
            return not line.empty();
        };

        for (auto& line : file | std::views::filter(isLineNotEmpty)) {
            std::cout << "Processing line: " << line << '\n';
            auto rawTokens = core::split(line, ' ');

            std::cout << rawTokens.size() << '\n';

            for (auto& rawToken : rawTokens) {

                std::cout << "Parsing token: " << rawToken << '\n';
                tokens.push_back(parseToken(rawToken));

                auto& lastToken = tokens.back();
                const auto n = lastToken.value.size();

                if (n != 1 && (lastToken.value[n - 1] == ':' || lastToken.value[n - 1] == ',')) {
                    tokens.push_back(parseToken(std::string { lastToken.value[n - 1] }));
                    lastToken.value.pop_back();
                }
            }
        }

        return tokens;
    }

private:
    Token parseToken(const std::string& rawToken) {
        static const std::string header = "#model";

        if (rawToken == "{") {
            return Token { TokenType::openingBrace, "{" };
        } else if (rawToken == "}") {
            return Token { TokenType::closingBrace, "}" };
        } else if (rawToken == header) {
            return Token { TokenType::header, header };
        } else if (rawToken == ",") {
            return Token { TokenType::coma, "," };
        } else if (rawToken == ":") {
            return Token { TokenType::colon, ":" };
        }

        validateString(rawToken);
        return Token { TokenType::string, rawToken };
    }

    bool isTokenConstraint(const std::string& rawToken) {
        return false;
    }

    void validateString(const std::string& string) {
        static auto isCharacterAllowed = [](const char character) {
            static const std::string allowedCharacters =
                "abcdefghijklmnoprstuwzxvABCDEFGHIJKLMNOPRSTUWZXV,:";

            return std::ranges::any_of(allowedCharacters,
                [&](const char allowedChar) { return character == allowedChar; });
        };

        if (not std::ranges::all_of(string, isCharacterAllowed))
            throw SyntaxError { fmt::format("String {} is not valid", string) };
    }
};

struct Model {
    struct Field {
        std::string name;
        std::string type;
    };

    inline static std::unordered_map<std::string, std::string> allowedTypes = {
        { "int", "int" },
        { "bool", "bool" },
        { "str", "std::string" },
        { "numeric", "float" }
    };

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

class Parser {
public:
    explicit Parser(const Tokenizer::Tokens& tokens)
        : m_currentIndex(0)
        , m_tokensSize(tokens.size())
        , m_tokens(tokens) {
    }

    Structures parseTokens() && {
        if (m_tokens[0].type != TokenType::header)
            throw SyntaxError { "Model file must start with proper header" };

        ++m_currentIndex;
        processTokens();

        return std::move(m_structures);
    }

private:
    void processTokens() {
        if (m_currentIndex == m_tokensSize)
            return;

        auto currentToken = getNextToken();

        if (currentToken->type == TokenType::string) {
            if (currentToken->value == "model")
                processModel();
            else if (currentToken->value == "enum")
                processEnum();
            else
                fail(fmt::format("Invalid symbol: {}", currentToken->value));

            return processTokens();
        }

        fail(fmt::format("Invalid symbol: {}", currentToken->value));
    }

    void processModel() {
        auto name = getNextToken(TokenType::string)->value;

        expectToken(TokenType::openingBrace);

        Model model { .name = name };

        while (nextToken()->type != TokenType::closingBrace)
            model.fields.push_back(processModelField());
        expectToken(TokenType::closingBrace);

        m_structures.models.push_back(std::move(model));
    }

    Model::Field processModelField() {
        auto name = getNextToken(TokenType::string)->value;
        expectToken(TokenType::colon);
        auto type = getNextToken(TokenType::string)->value;
        expectToken(TokenType::coma);

        if (not Model::allowedTypes.contains(type))
            fail("Invalid type: " + type);

        return Model::Field { name, Model::allowedTypes.at(type) };
    }

    void processEnum() {
        auto name = getNextToken(TokenType::string)->value;

        expectToken(TokenType::openingBrace);

        Enum enumerate { .name = name };

        while (nextToken()->type != TokenType::closingBrace)
            enumerate.values.push_back(processEnumField());
        expectToken(TokenType::closingBrace);

        m_structures.enums.push_back(std::move(enumerate));
    }

    std::string processEnumField() {
        auto value = getNextToken(TokenType::string)->value;
        expectToken(TokenType::coma);

        return value;
    }

    Token const* nextToken() {
        if (m_currentIndex == m_tokensSize)
            fail("Unexpected end of file");

        return &m_tokens[m_currentIndex];
    }

    Token const* getNextToken() {
        if (m_currentIndex == m_tokensSize)
            fail("Unexpected end of file");

        return &m_tokens[m_currentIndex++];
    }

    Token const* getNextToken(TokenType tokenType) {
        auto token = getNextToken();

        if (token->type != tokenType)
            fail(fmt::format("Unexpected symbol {}", token->value));

        return token;
    }

    void expectToken(TokenType tokenType) {
        if (auto token = getNextToken(); token->type != tokenType)
            fail(fmt::format("Unexpected symbol {}", token->value));
    }

    void fail(const std::string& reason) {
        throw SyntaxError { reason };
    }

    Structures m_structures;

    int m_currentIndex;
    int m_tokensSize;
    const Tokenizer::Tokens& m_tokens;
};

static const std::string fourSpaces = "    ";

class Generator {
public:
    struct File {
        std::string content;
        std::string path;
    };

    using Files = std::vector<File>;

    Files generateCode(const Structures& structures) {
        Files files;

        for (auto& enumerate : structures.enums)
            files.emplace_back(
                generateEnum(enumerate), enumerate.name + ".hpp");

        for (auto& model : structures.models)
            files.emplace_back(
                generateModel(model), model.name + ".hpp");

        return files;
    }

private:
    std::string generateModel(const Model& model) {
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

    std::string generateModelFromJson(const Model& model) {
        std::ostringstream data;

        data << fourSpaces << "static " << model.name << " fromJson(const " << jsonType << "& json) {\n"
             << fourSpaces << fourSpaces << "return "
             << "JSON {};\n"
             << fourSpaces << "}\n";

        return data.str();
    }

    std::string generateModelToJson(const Model& model) {
        std::ostringstream data;

        data << fourSpaces << jsonType << " toJson(const " << model.name << "& model) {\n";
        data << fourSpaces << fourSpaces << "return " << model.name << " {};\n";
        data << fourSpaces << "}\n";

        return data.str();
    }

    std::string generateEnum(const Enum& enumerate) {
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

    std::string generateEnumToString(const Enum& enumerate) {
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
};

int main(int argc, char** argv) {
    ASSERT(argc == 2, "Wrong number of arguments");

    // for now, 1st argument is a path to the file
    std::string filePath(argv[1]);
    core::FileSystem fs;

    auto fileContent = fs.readLines(filePath);

    try {
        auto tokens = Tokenizer {}.tokenize(fileContent);

        auto structures = Parser { tokens }.parseTokens();
        auto files = Generator {}.generateCode(structures);

        std::cout << "\n\n$ Generated files:\n\n";

        for (auto& [fileContent, fileName] : files) {
            std::cout << "[" << fileName << "]\n"
                      << "\n"
                      << fileContent << "";
        }

    } catch (ModelGeneratorError& error) {
        std::cout << error.asString() << '\n';
    }

    return 0;
}