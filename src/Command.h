#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

enum class RedirectType {
    in,     // <
    out,    // >
    append, // >>
    none,
};

RedirectType stringToRedirectType(const std::string& str);

struct Redirect {
    RedirectType type;
    std::string target;
};

struct CommandNode {
    std::string name;
    std::vector<std::string> args;
    std::vector<Redirect> redirects;
};

void echoCmd(const CommandNode& command);

extern const std::unordered_map<std::string, std::function<void(const CommandNode&)>> command_map;