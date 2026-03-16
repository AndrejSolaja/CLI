#pragma once

#include <string>

class Console
{
public:
    static Console& getInstance();

    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    void run();
    void setPromptSymbol(std::string new_symbol);

private:
    Console() = default;

    std::string prompt_symbol = "$";
};

