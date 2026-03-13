#pragma once

#include <string>
#include <vector>
#include <optional>

enum class TokenizerErrorType {
    unterminated_quote,
};

enum class TokenType {
    string,         // words, commands, arguments, file paths
    pipe,           // |
    redirect,       // <, >, >>
    end,            // \n
    error,
};

std::string tokenTypeName(TokenType type);

struct Token {
    TokenType type;
    std::string value;

    std::optional<TokenizerErrorType> error_type;
    size_t error_offset_within_token = 0;

    Token() {}
    Token(TokenType type) : type(type) {}
};

class Tokenizer
{
public:
    Tokenizer(const std::string& file_path);

    std::vector<Token> tokenize();
    std::string tokenString(const Token& token);

private:
    bool loadFile(const std::string& file_path);

    std::string text;       // input data
    size_t cursor;          // current position
    size_t end;             // end of input
    bool has_next;

    Token makeToken(TokenType type, size_t token_start);
    Token makeError(TokenizerErrorType type, size_t token_start, size_t error_loc);
    Token readString();
    Token next();
    void resetTokenizer();

    static bool isTokenSeparator(char c);
};

