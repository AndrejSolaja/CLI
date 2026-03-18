#pragma once
#include "DataTypes.h"

#include <string>
#include <vector>
#include <optional>


class Tokenizer
{
public:
    Tokenizer(const std::string& text);

    std::vector<CLI::Token> tokenize();
    
private:
    bool loadFile(const std::string& file_path);

    std::string text;       // input data
    size_t cursor;          // current position
    size_t end;             // end of input

    CLI::Token makeToken(CLI::TokenType type, size_t token_start);
    CLI::Token makeError(CLI::TokenizerErrorType type, size_t token_start, size_t error_loc);
    CLI::Token readString();
    CLI::Token next();
    void resetTokenizer();

    static bool isTokenSeparator(char c);
};

