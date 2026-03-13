#include "Parser.h"
#include <iostream>

Parser::Parser(Tokenizer tokenizer) {
    for (auto x : tokenizer.tokenize()) {
        tokens.push(x);
    }
}

std::vector<CommandNode> Parser::parseInput() {
    std::vector<CommandNode> pipeline_commands;
    while (tokens.front().type != TokenType::end) {
        pipeline_commands.push_back(parseCommand());
    }
    return pipeline_commands;
}

CommandNode Parser::parseCommand() {
    CommandNode command;

    if (tokens.front().type != TokenType::string) {
        //TODO
        std::cout << "ERROR:" << "First token in command isnt command name" << std::endl;
    }
    command.name = tokens.front().value; tokens.pop();

    while (!tokens.empty() && tokens.front().type != TokenType::end) {
        Token tok = tokens.front(); tokens.pop();

        if (tok.type == TokenType::redirect) {
            if (tokens.empty()) {
                //TODO
                std::cout << "ERROR:" << "No target for redirection" << std::endl;
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
