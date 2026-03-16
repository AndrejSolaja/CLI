#pragma once
#include "DataTypes.h"

#include <iostream>
#include <fstream>
#include <vector>

class Executor
{
public:
    Executor(const std::vector<CommandNode>& commands);
    void executeCommands();

private:
    std::vector<CommandNode> commands;
    std::streambuf* saved_input  = std::cin.rdbuf();
    std::streambuf* saved_output = std::cout.rdbuf();
    std::ifstream input_stream;
    std::ofstream output_stream;

    void resolveRedirections(CommandNode& command);
    void saveStreams();
    void restoreStreams();
    void executeSingleCommand(CommandNode& command);
    void executePipedCommands(std::vector<CommandNode>& commands);
};

