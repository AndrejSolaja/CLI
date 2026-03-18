#include "Parser.h"

#include <iostream>
using namespace CLI;

RedirectType Parser::stringToRedirectType(const std::string& str) {
    if (str == "<") {
        return RedirectType::in;
    } else if (str == ">") {
        return RedirectType::out;
    } else if (str == ">>") {
        return RedirectType::append;
    }
    return RedirectType::none;
}

Parser::Parser(const std::vector<Token>& tokens) {
    for (auto x : tokens) {
        this->tokens.push(x);
    }
}

std::vector<CommandNode> Parser::parse() {
    std::vector<CommandNode> pipeline_commands;
    while (tokens.front().type != TokenType::end) {
        pipeline_commands.push_back(parseCommand());
    }
    return pipeline_commands;
}

CommandNode Parser::parseCommand() {
    CommandNode command;

    if (tokens.front().type != TokenType::string) {
        std::cerr << "First token in command isn't command name" << std::endl;
    }
    command.name = tokens.front().value; tokens.pop();

    while (!tokens.empty() && tokens.front().type != TokenType::end) {
        Token tok = tokens.front(); tokens.pop();

        if (tok.type == TokenType::redirect) {
            if (tokens.empty()) {
                std::cerr << "No target for redirection" << std::endl;
            } else {
                Token target = tokens.front(); tokens.pop();
                command.redirects.push_back({stringToRedirectType(tok.value), target.value});
            }
        } else if (tok.type == TokenType::pipe) {
            // Pipe is consumed, can go to next command
            break;
        } else {
            // argument
            command.args.push_back(tok.value);
        }
    }

    return command;
}
