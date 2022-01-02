#pragma once

#include "Language.h"
#include "Tokenizer.h"

class Parser {
public:
    explicit Parser(const Tokenizer::Tokens& tokens);

    Structures parseTokens() &&;

private:
    void processTokens();
    void processModel(bool isMessage);

    Model::Field processModelField();

    void processEnum();

    std::string processEnumField();

    Token const* nextToken();

    Token const* getNextToken();
    Token const* getNextToken(TokenType tokenType);

    void expectToken(TokenType tokenType);

    void fail(const std::string& reason);

    Structures m_structures;

    int m_currentIndex;
    int m_tokensSize;
    const Tokenizer::Tokens& m_tokens;
};