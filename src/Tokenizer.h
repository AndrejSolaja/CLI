#pragma once

#include <string>
#include <vector>
#include <optional>

enum class tokenizer_error_t {
    unterminated_quote,
};

enum class token_type_t {
    string,         // words, commands, arguments, file paths
    pipe,           // |
    redirect,       // <, >, >>
    end,            // \n
    error,
};

std::string token_type_name(token_type_t type);

struct token_t {
    token_type_t type;
    size_t offset;
    size_t length;

    std::optional<tokenizer_error_t> error_type;
    size_t error_offset_within_token;

    token_t() {}
    token_t(token_type_t type) : type(type) {}
};

class Tokenizer
{
public:
    Tokenizer(const std::string& file_path);

    std::vector<token_t> tokenize();
    std::string token_string(token_t token);

private:
    bool load_file(const std::string& file_path);

    std::string raw_data;   // input data
    size_t cursor;          // current position
    size_t end;             // end of input
    bool has_next;

    token_t make_token(token_type_t type, size_t token_start);
    token_t make_error(tokenizer_error_t error_type, size_t token_start, size_t error_loc);
    token_t read_string();
    token_t next();
    void reset_tokenizer();

    static bool is_token_seperator(char c);
};

