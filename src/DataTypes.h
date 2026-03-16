#pragma once

#include <string>
#include <optional>
#include <vector>

// ============= TOKENS =================

enum class TokenizerErrorType {
    unterminated_quote,
};
// Changed from TokenType to TokenType_t cause of <windows> header
enum class TokenType_t {
    string,         // words, commands, arguments, file paths
    pipe,           // |
    redirect,       // <, >, >>
    end,            // \n
    error,
};

struct Token {
    TokenType_t type;
    std::string value;

    std::optional<TokenizerErrorType> error_type;
    size_t error_offset_within_token = 0;

    Token() {}
    Token(TokenType_t type) : type(type) {}
};

// ========== COMMANDS ==================

enum class RedirectType {
    in,     // <
    out,    // >
    append, // >>
    none,
};

struct Redirect {
    RedirectType type;
    std::string target;
};

struct CommandNode {
    std::string name;
    std::vector<std::string> args;
    std::vector<Redirect> redirects;
};