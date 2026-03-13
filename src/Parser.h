#pragma once
#include "Tokenizer.h"
#include <string>
#include <queue>
#include <vector>

enum class RedirectType {
    in,     // <
    out,    // >
    append, // >>
    none,   
};

RedirectType stringToRedirectType(const std::string str);

struct Redirect {
    RedirectType type;
    std::string target;
};

struct CommandNode {
    std::string name;
    std::vector<std::string> args;
    std::vector<Redirect> redirects;
};

class Parser
{
public:
    Parser(Tokenizer tokenizer);
    std::vector<CommandNode> parseInput();

private:
    std::queue<Token> tokens;
    CommandNode parseCommand();
};

