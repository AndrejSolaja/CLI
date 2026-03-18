#include "Executor.h"
#include "Command.h"

#include <iostream>
#include <sstream>
using namespace CLI;


Executor::Executor(const std::vector<CommandNode>& commands) {
    this->commands = commands;
}

void Executor::executeCommands() {
    if (commands.size() == 1) {
        executeSingleCommand(commands[0]);
    } else {
        executePipedCommands(commands);
    }
}

void Executor::resolveRedirections(CommandNode& command) {
    for (auto redirect : command.redirects) {
        if (redirect.type == RedirectType::in) {
            // Not sure if cin flush is needed
            // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            input_stream.open(redirect.target);
            if (!input_stream.is_open()) {
                std::cerr << "File " << redirect.target << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::cin.rdbuf(input_stream.rdbuf());
        }
        else if (redirect.type == RedirectType::out) {
            std::cout.flush();
            output_stream.open(redirect.target);
            if (!output_stream.is_open()) {
                std::cerr << "File " << redirect.target << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::cout.rdbuf(output_stream.rdbuf());
        }
        else if (redirect.type == RedirectType::append) {
            std::cout.flush();
            output_stream.open(redirect.target, std::ios::app);
            if (!output_stream.is_open()) {
                std::cerr << "File " << redirect.target << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::cout.rdbuf(output_stream.rdbuf());
        }
    }
}

void Executor::saveStreams() {
    saved_input = std::cin.rdbuf();
    saved_output = std::cout.rdbuf();
}

void Executor::restoreStreams() {
    // in
    // Not sure if cin flush is needed
    // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.rdbuf(saved_input);
    if (input_stream.is_open()) {
        input_stream.close();
    }

    // out
    std::cout.flush();
    std::cout.rdbuf(saved_output);
    if (output_stream.is_open()) {
        output_stream.close();
    }
}

void Executor::executeSingleCommand(CommandNode& command) {
    saveStreams();
    resolveRedirections(command);

    auto it = command_map.find(command.name);
    if (it != command_map.end()) {
        it->second(command);
    } else {
        std::cerr << "Unknown command: " << command.name << std::endl;
    }

    restoreStreams();
}

void Executor::executePipedCommands(std::vector<CommandNode>& commands) {
    saveStreams();
    // Init streams between commands
    std::vector<std::stringstream> pipe_streams(commands.size() - 1);

    // redirect cin/cout, reslove redirections for first and last command
    // Error of using redirections in between or wrong ones on start and end is not handled
    for(int i = 0 ; i < commands.size(); i++){
        if(i == 0){
            // First command
            std::cout.rdbuf(pipe_streams[i].rdbuf());

            resolveRedirections(commands[i]);

        }else if(i == commands.size() - 1){
            // Last command
            std::cout.rdbuf(saved_output);

            std::cin.rdbuf(pipe_streams[i - 1].rdbuf());

            resolveRedirections(commands[i]);

        }else{
            // In between
            std::cout.rdbuf(pipe_streams[i].rdbuf());

            std::cin.rdbuf(pipe_streams[i - 1].rdbuf());
            

        }
        // execute
        auto it = command_map.find(commands[i].name);
        if (it != command_map.end()) {
            it->second(commands[i]);
        } else {
            std::cerr << "Unknown command: " << commands[i].name << std::endl;
        }


    }

    restoreStreams();

}
