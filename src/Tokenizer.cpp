#include "Tokenizer.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

std::string token_type_name(token_type_t type) {
    switch (type) {
        case token_type_t::string:   return "string";
        case token_type_t::pipe:     return "pipe";
        case token_type_t::redirect: return "redirect";
        case token_type_t::end:      return "end";
        case token_type_t::error:    return "error";
        default:                     return "unknown";
    }
}

// ---- Tokenizer ----

Tokenizer::Tokenizer(const std::string& file_path) {
    if (!load_file(file_path)) {
        throw std::runtime_error("Could not open file " + file_path);
    }
}

bool Tokenizer::load_file(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    raw_data = buffer.str();

    cursor = 0;
    end = raw_data.size();
    has_next = true;

    return true;
}

token_t Tokenizer::make_token(token_type_t type, size_t token_start) {
    token_t new_token;
    new_token.type = type;
    new_token.offset = token_start;
    new_token.length = cursor - token_start;
    return new_token;
}

token_t Tokenizer::make_error(tokenizer_error_t error_type, size_t token_start, size_t error_loc) {
    token_t new_token;
    new_token.type = token_type_t::error;
    new_token.offset = token_start;
    new_token.length = cursor - token_start;
    new_token.error_type = error_type;
    new_token.error_offset_within_token = error_loc - token_start; // offset from token start
    return new_token;
}

token_t Tokenizer::read_string() {
    /*
        Things that count as string:

        hello
        "Hello world"
        \n  - only in multiline string, usual \n at the end of command get parsed as end token
        input.txt
    */

    size_t token_start = cursor;
    bool in_single_quote = false;
    bool in_double_quote = false;

    while (cursor < end) {
        char c = raw_data[cursor];

        // ---- SINGLE QUOTE STATE ----
        if (in_single_quote) {
            if (c == '\'') in_single_quote = false;
            cursor++;
            continue;
        }

        // ---- DOUBLE QUOTE STATE ----
        if (in_double_quote) {
            if (c == '"') in_double_quote = false;
            cursor++;
            continue;
        }

        // ---- NORMAL STATE ----
        switch (c) {
            case '\'': in_single_quote = true; break;
            case '"':  in_double_quote = true; break;
            default:
                if (is_token_seperator(c)) goto done;
        }

        cursor++;
    }

    done:
    // Check for unterminated quotes
    if (in_single_quote || in_double_quote) {
        return make_error(tokenizer_error_t::unterminated_quote, token_start, cursor);
    }

    return make_token(token_type_t::string, token_start);
}

bool Tokenizer::is_token_seperator(char c) {
    switch (c) {
        case ' ':
        case '\t':
        case '\n':
        case '|':
        case '>':
        case '<':
            return true;
        default:
            return false;
    }
}

token_t Tokenizer::next() {
    if (!has_next) {
        return token_t(token_type_t::end);
    }

    // Skip whitespace - but NOT newlines
    // isspace expects value in range 0 to 255, thats why cast is necessary
    while (cursor < end && isspace((unsigned char)raw_data[cursor]) && raw_data[cursor] != '\n') {
        cursor++;
    }

    // Check again after skipping whitespaces
    if (cursor >= end) {
        has_next = false;
        return token_t(token_type_t::end);
    }

    size_t token_start = cursor;
    char c = raw_data[cursor];

    switch (c) {
        case '\n':
            cursor++;
            has_next = false;
            return make_token(token_type_t::end, token_start);

        case '|':
            cursor++;
            return make_token(token_type_t::pipe, token_start);

        case '>':
        case '<':
            if (raw_data[cursor] == '>' && cursor + 1 < end && raw_data[cursor + 1] == '>') {
                cursor++;
            }
            cursor++;
            return make_token(token_type_t::redirect, token_start);

        default:
            return read_string();
    }
}

std::vector<token_t> Tokenizer::tokenize() {
    /*
        Does a whole pass and returns a flat list of tokens.
    */
    std::vector<token_t> token_list;

    while (has_next) {
        token_t token = next();

        if (token.error_type.has_value()) {
            // Print error  TODO: Expand on this with better message
            std::cout << "Error at token \"" << raw_data.substr(token.offset, token.length)
                      << "\" on position " << token.error_offset_within_token << std::endl;
            return {};
        }

        token_list.push_back(token);
    }

    // Reset tokenizer so we can run it again if needed
    reset_tokenizer();
    return token_list;
}

std::string Tokenizer::token_string(token_t token) {
    std::string text = raw_data.substr(token.offset, token.length);
    std::string type = token_type_name(token.type);
    return "[" + text + "](" + type + ")";
}

void Tokenizer::reset_tokenizer() {
    cursor = 0;
    has_next = true;
}
