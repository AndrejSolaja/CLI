#include "Tokenizer.h"

#include <fstream>
using namespace CLI;
#include <sstream>
#include <iostream>
#include <stdexcept>

Tokenizer::Tokenizer(const std::string& text) {
    this->text = text;
    cursor = 0;
    end = text.size();
}

bool Tokenizer::loadFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        return false;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    text = buffer.str();

    cursor = 0;
    end = text.size();

    return true;
}

Token Tokenizer::makeToken(TokenType type, size_t token_start) {
    Token new_token;
    new_token.type = type;
    new_token.value = text.substr(token_start, cursor - token_start);
    return new_token;
}

Token Tokenizer::makeError(TokenizerErrorType error_type, size_t token_start, size_t error_loc) {
    Token new_token;
    new_token.type = TokenType::error;
    new_token.value = text.substr(token_start, cursor - token_start);
    new_token.error_type = error_type;
    new_token.error_offset_within_token = error_loc - token_start; // offset from token start
    return new_token;
}

Token Tokenizer::readString() {
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
        char c = text[cursor];

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
                if (isTokenSeparator(c)) goto done;
        }

        cursor++;
    }

    done:
    if (in_single_quote || in_double_quote) {
        return makeError(TokenizerErrorType::unterminated_quote, token_start, cursor);
    }

    return makeToken(TokenType::string, token_start);
}

bool Tokenizer::isTokenSeparator(char c) {
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

Token Tokenizer::next() {
    // Skip whitespace - but NOT newlines
    // isspace expects value in range 0 to 255, thats why cast is necessary
    while (cursor < end && isspace((unsigned char)text[cursor]) && text[cursor] != '\n') {
        cursor++;
    }

    if (cursor >= end) {
        return Token(TokenType::end);
    }

    size_t token_start = cursor;
    char c = text[cursor];

    switch (c) {
        case '\n':
            cursor++;
            return makeToken(TokenType::end, token_start);

        case '|':
            cursor++;
            return makeToken(TokenType::pipe, token_start);

        case '>':
        case '<':
            if (text[cursor] == '>' && cursor + 1 < end && text[cursor + 1] == '>') {
                cursor++;
            }
            cursor++;
            return makeToken(TokenType::redirect, token_start);

        default:
            return readString();
    }
}

std::vector<Token> Tokenizer::tokenize() {
    /*
        Does a whole pass and returns a flat list of tokens.
    */
    std::vector<Token> token_list;

    while (true) {
        Token token = next();

        if (token.error_type.has_value()) {
            std::cerr << "Error at token \"" << token.value
                      << "\" on position " << token.error_offset_within_token << std::endl;
            return {};
        }

        token_list.push_back(token);
        if (token.type == TokenType::end) break;
    }

    // Reset tokenizer so we can run it again if needed
    resetTokenizer();
    return token_list;
}

void Tokenizer::resetTokenizer() {
    cursor = 0;
}

