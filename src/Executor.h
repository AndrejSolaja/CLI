#pragma once
#include "DataTypes.h"

#include <iostream>
#include <fstream>
#include <vector>

class Executor
{
public:
    Executor(const std::vector<CLI::CommandNode>& commands);
    void executeCommands();

private:
    std::vector<CLI::CommandNode> commands;
    std::streambuf* saved_input  = std::cin.rdbuf();
    std::streambuf* saved_output = std::cout.rdbuf();
    std::ifstream input_stream;
    std::ofstream output_stream;

    void resolveRedirections(CLI::CommandNode& command);
    void saveStreams();
    void restoreStreams();
    void executeSingleCommand(CLI::CommandNode& command);
    void executePipedCommands(std::vector<CLI::CommandNode>& commands);
};

