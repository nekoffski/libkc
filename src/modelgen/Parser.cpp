#include "Parser.h"

#include "Core.h"
#include "kc/core/Log.h"

#include <iostream>

Parser::Parser(const Tokenizer::Tokens& tokens)
    : m_currentIndex(0), m_tokensSize(tokens.size()), m_tokens(tokens) {}

Structures Parser::parseTokens() && {
    if (m_tokens[0].type != TokenType::header)
        throw SyntaxError{"Model file must start with proper header"};

    ++m_currentIndex;
    processTokens();

    return std::move(m_structures);
}

void Parser::processTokens() {
    if (m_currentIndex == m_tokensSize) return;

    auto currentToken = getNextToken();

    if (currentToken->type == TokenType::header) return processTokens();

    if (currentToken->type == TokenType::string) {
        const auto isMessage = currentToken->value == "message";

        if (currentToken->value == "model" || isMessage)
            processModel(isMessage);
        else if (currentToken->value == "enum")
            processEnum();
        else
            fail(fmt::format("Invalid symbol: {}", currentToken->value));

        return processTokens();
    }

    fail(fmt::format("Invalid symbol: {}", currentToken->value));
}

void Parser::processModel(bool isMessage) {
    auto name = getNextToken(TokenType::string)->value;

    expectToken(TokenType::openingBrace);

    Model model{.isMessage = isMessage, .name = name};

    while (nextToken()->type != TokenType::closingBrace)
        model.fields.push_back(processModelField());
    expectToken(TokenType::closingBrace);

    m_structures.models.push_back(std::move(model));
}

Model::Field Parser::processModelField() {
    auto name = getNextToken(TokenType::string)->value;
    expectToken(TokenType::colon);
    auto typeDescription = getNextToken(TokenType::string)->value;
    expectToken(TokenType::semicolon);

    std::cout << "kcz" << typeDescription << '\n';

    return Model::Field{name, typeDescription};
}

void Parser::processEnum() {
    auto name = getNextToken(TokenType::string)->value;

    expectToken(TokenType::openingBrace);

    Enum enumerate{.name = name};

    while (nextToken()->type != TokenType::closingBrace)
        enumerate.values.push_back(processEnumField());
    expectToken(TokenType::closingBrace);

    m_structures.enums.push_back(std::move(enumerate));
}

std::string Parser::processEnumField() {
    auto value = getNextToken(TokenType::string)->value;
    expectToken(TokenType::semicolon);

    return value;
}

Token const* Parser::nextToken() {
    if (m_currentIndex == m_tokensSize) fail("Unexpected end of file");

    return &m_tokens[m_currentIndex];
}

Token const* Parser::getNextToken() {
    if (m_currentIndex == m_tokensSize) fail("Unexpected end of file");

    return &m_tokens[m_currentIndex++];
}

Token const* Parser::getNextToken(TokenType tokenType) {
    auto token = getNextToken();

    if (token->type != tokenType) fail(fmt::format("Unexpected symbol {}", token->value));

    return token;
}

void Parser::expectToken(TokenType tokenType) {
    if (auto token = getNextToken(); token->type != tokenType)
        fail(fmt::format("Unexpected symbol {}", token->value));
}

void Parser::fail(const std::string& reason) { throw SyntaxError{reason}; }
