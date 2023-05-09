#pragma once

#include <string>
#include <vector>

enum class TokenType : unsigned char {
    header,
    string,
    colon,
    openingBrace,
    closingBrace,
    semicolon,
    constraint
};

std::string toString(TokenType tokenType);

struct Token {
    TokenType type;
    std::string value;
};

class Tokenizer {
   public:
    using Tokens = std::vector<Token>;

    Tokens tokenize(const std::vector<std::string>& file);

   private:
    Token parseToken(const std::string& rawToken);

    bool isTokenConstraint(const std::string& rawToken);

    void validateString(const std::string& string);
};