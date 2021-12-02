#include <iostream>

#include <stack>

#include <ranges>

#include <kc/core/ErrorBase.hpp>
#include <kc/core/FileSystem.h>
#include <kc/core/Log.h>

DEFINE_ERROR(ModelGeneratorError);
DEFINE_SUB_ERROR(SyntaxError, ModelGeneratorError);

using namespace kc;

enum class OutputType {
    singleFile,
    multiFile
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
            for (auto& rawToken : core::split(line, ' ')) {
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
    std::string name;
};

struct Enum {
    std::string name;
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

        while (getNextToken()->type != TokenType::closingBrace)
            processModelField();

        m_structures.models.emplace_back(name);
    }

    void processModelField() {
    }

    void processEnum() {
        auto name = getNextToken(TokenType::string)->value;

        expectToken(TokenType::colon);

        auto type = getNextToken(TokenType::string)->value;

        expectToken(TokenType::openingBrace);

        while (getNextToken()->type != TokenType::closingBrace)
            processEnumField();

        m_structures.enums.emplace_back(name);
    }

    void processEnumField() {
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

class Generator {
};

int main(int argc, char** argv) {
    ASSERT(argc == 2, "Wrong number of arguments");

    // for now, 1st argument is a path to the file
    std::string filePath(argv[1]);
    core::FileSystem fs;

    auto fileContent = fs.readLines(filePath);

    try {
        auto tokens = Tokenizer {}.tokenize(fileContent);

        for (auto& token : tokens) {
            std::cout << token.value << '\n';
        }
        std::cout << "\n\n";

        auto structures = Parser { tokens }.parseTokens();

        std::cout << "[Enums]:\n";
        for (auto& en : structures.enums)
            std::cout << en.name << '\n';

        std::cout << "[Models]:\n";
        for (auto& model : structures.models)
            std::cout << model.name << '\n';

    } catch (ModelGeneratorError& error) {
        std::cout << error.asString() << '\n';
    }

    return 0;
}