#pragma once
#include "DataTypes.h"

#include <string>
#include <queue>
#include <vector>

class Parser
{
public:
    Parser(const std::vector<Token>& tokens);
    std::vector<CommandNode> parse();

private:
    std::queue<Token> tokens;
    CommandNode parseCommand();
    static RedirectType stringToRedirectType(const std::string& str);
};

