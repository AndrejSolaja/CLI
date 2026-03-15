#pragma once
#include "Parser.h"
#include <iostream>
#include <fstream>

class Executor
{
public:
    Executor(Parser parser);
    void executeCommands();

private:
    std::vector<CommandNode> commands;
    std::streambuf* saved_input  = std::cin.rdbuf();
    std::streambuf* saved_output = std::cout.rdbuf();
    std::ifstream input_stream;
    std::ofstream output_stream;

    void resolveRedirections();
    void restoreStreams();
    void executeSingleCommand(CommandNode& command);
    void executePipedCommands();
};

