#pragma once
#include "Tokenizer.h"
#include "Command.h"
#include <string>
#include <queue>
#include <vector>

class Parser
{
public:
    Parser(Tokenizer tokenizer);
    std::vector<CommandNode> parseInput();

private:
    std::queue<Token> tokens;
    CommandNode parseCommand();
};

