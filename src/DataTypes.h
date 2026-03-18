#pragma once

#include <string>
#include <optional>
#include <vector>

namespace CLI {

// ============= TOKENS =================

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

struct Token {
    TokenType type;
    std::string value;

    std::optional<TokenizerErrorType> error_type;
    size_t error_offset_within_token = 0;

    Token() {}
    Token(TokenType type) : type(type) {}

    std::string toString() const {
        const char* type_str;
        switch (type) {
            case TokenType::string:   type_str = "string";   break;
            case TokenType::pipe:     type_str = "pipe";     break;
            case TokenType::redirect: type_str = "redirect"; break;
            case TokenType::end:      type_str = "end";      break;
            case TokenType::error:    type_str = "error";    break;
            default:                  type_str = "unknown";  break;
        }
        return "[" + value + "](" + type_str + ")";
    }
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

} // namespace CLI