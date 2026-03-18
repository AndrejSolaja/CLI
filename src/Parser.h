#pragma once
#include "DataTypes.h"

#include <string>
#include <queue>
#include <vector>

class Parser
{
public:
    Parser(const std::vector<CLI::Token>& tokens);
    std::vector<CLI::CommandNode> parse();

private:
    std::queue<CLI::Token> tokens;
    CLI::CommandNode parseCommand();
    static CLI::RedirectType stringToRedirectType(const std::string& str);
};

