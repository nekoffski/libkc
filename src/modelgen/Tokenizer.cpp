#include "Tokenizer.h"

#include <iostream>

#include <ranges>

#include "kc/core/Log.h"
#include "kc/core/String.h"

#include "Core.h"

using namespace kc;

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

Tokenizer::Tokens Tokenizer::tokenize(const std::vector<std::string>& file) {
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

Token Tokenizer::parseToken(const std::string& rawToken) {
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

bool Tokenizer::isTokenConstraint(const std::string& rawToken) {
    return false;
}

void Tokenizer::validateString(const std::string& string) {
    static auto isCharacterAllowed = [](const char character) {
        static const std::string allowedCharacters =
            "abcdefghijklmnoprstuwzxvABCDEFGHIJKLMNOPRSTUWZXV,:";

        return std::ranges::any_of(allowedCharacters,
            [&](const char allowedChar) { return character == allowedChar; });
    };

    if (not std::ranges::all_of(string, isCharacterAllowed))
        throw SyntaxError { fmt::format("String {} is not valid", string) };
}