#pragma once
#include "DataTypes.h"

#include <string>
#include <vector>
#include <optional>


class Tokenizer
{
public:
    Tokenizer(const std::string& text);

    std::vector<Token> tokenize();
    static std::string tokenToString(const Token& token);
    
private:
    bool loadFile(const std::string& file_path);

    std::string text;       // input data
    size_t cursor;          // current position
    size_t end;             // end of input
    bool has_next;

    Token makeToken(TokenType_t type, size_t token_start);
    Token makeError(TokenizerErrorType type, size_t token_start, size_t error_loc);
    Token readString();
    Token next();
    void resetTokenizer();

    static std::string tokenTypeEnumToString(const TokenType_t type);
    static bool isTokenSeparator(char c);
};

