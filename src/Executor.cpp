#include "Executor.h"
#include <iostream>

Executor::Executor(Parser parser) {
    this->commands = parser.parseInput();
}

void Executor::executeCommands() {
    if (commands.size() == 1) {
        executeSingleCommand(commands[0]);
    } else {
        executePipedCommands();
    }
}

void Executor::resolveRedirections() {
    for (auto redirect : commands[0].redirects) {
        if (redirect.type == RedirectType::in) {
            // Not sure if cin flush is needed
            // std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            saved_input = std::cin.rdbuf();
            input_stream.open(redirect.target);
            if (!input_stream.is_open()) {
                std::cerr << "File " << redirect.target << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::cin.rdbuf(input_stream.rdbuf());
        }
        else if (redirect.type == RedirectType::out) {
            std::cout.flush();
            saved_output = std::cout.rdbuf();
            output_stream.open(redirect.target);
            std::cout.rdbuf(output_stream.rdbuf());
        }
        else if (redirect.type == RedirectType::append) {
            std::cout.flush();
            saved_output = std::cout.rdbuf();
            output_stream.open(redirect.target, std::ios::app);
            std::cout.rdbuf(output_stream.rdbuf());
        }
    }
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
    resolveRedirections();

    auto it = command_map.find(command.name);
    if (it != command_map.end()) {
        it->second(command);
    } else {
        std::cerr << "Unknown command: " << command.name << std::endl;
    }

    restoreStreams();
}

void Executor::executePipedCommands() {
    // TODO
}
