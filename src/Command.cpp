#include "Command.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <filesystem>

#include "Console.h"

RedirectType stringToRedirectType(const std::string& str) {
    if (str == "<") {
        return RedirectType::in;
    } else if (str == ">") {
        return RedirectType::out;
    } else if (str == ">>") {
        return RedirectType::append;
    }
    return RedirectType::none;
}

void echoCmd(const CommandNode& command) {
    std::ifstream input_stream;
    std::string buffer;

    if(command.args.size() > 1){
        std::cerr << "Command " << command.name << "takes at most 1 argument and has no options" << std::endl;
    }
    else if(command.args.size() == 1) {
        std::string arg = command.args[0];
        if(arg[0] == '"' && arg[arg.size() - 1] == '"') {
            buffer = arg.substr(1, arg.size() - 2);
        }else {
            // file path
            input_stream.open(command.args[0]);
            if (!input_stream.is_open()) {
                std::cerr << "File " << command.args[0] << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::cin.rdbuf(input_stream.rdbuf());

            // std::getline(std::cin, buffer);
            std::ostringstream ss;
            ss << input_stream.rdbuf();
            buffer = ss.str();
        }
   
    }else if(command.args.size() == 0) {
        std::getline(std::cin, buffer);
    }
    
    std::cout << buffer;

}

void promptCmd(const CommandNode& command) {
    if(command.args.size() != 1){
        std::cerr << "Command " << command.name << "takes 1 argument and has no options" << std::endl;
    }
    std::string arg = command.args[0];
    if(arg[0] == '"' && arg[arg.size() - 1] == '"') {
            Console::getInstance().setPromptSymbol(arg.substr(1, arg.size() - 2));
    }else{
        std::cerr << "Command " << command.name << "argument must be enclosed in double quotes" << std::endl;
    }

}

void timeCmd(const CommandNode& command) {
    if(command.args.size() != 0) {
        std::cerr << "Command " << command.name << "takes no argument and has no options" << std::endl;
    }

    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    tm* local = std::localtime(&t);

    std::cout << std::put_time(local, "%H:%M:%S");

}

void dateCmd(const CommandNode& command) {
    if(command.args.size() != 0) {
        std::cerr << "Command " << command.name << "takes no argument and has no options" << std::endl;
    }

    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    tm* local = std::localtime(&t);

    std::cout << std::put_time(local, "%Y-%m-%d");

}

void touchCmd(const CommandNode& command) {
    if(command.args.size() != 1) {
        std::cerr << "Command " << command.name << "takes 1 argument and has no options" << std::endl;
    }
    std::string file_path = command.args[0];

    std::ifstream existing_file(file_path);
    if (existing_file.is_open()) {
        std::cerr << "File " << file_path << " already exists." << std::endl;
        return;
    }
    // std::ios::out (default) mode creates new file, if it already exists , trunactes it
    std::ofstream new_file(file_path, std::ios::out);


}

void truncateCmd(const CommandNode& command) {
    if(command.args.size() != 1) {
        std::cerr << "Command " << command.name << "takes 1 argument and has no options" << std::endl;
    }
    std::string file_path = command.args[0];

    std::ifstream existing_file(file_path);
    if (!existing_file.is_open()) {
        std::cerr << "File " << file_path << " cannot be found." << std::endl;
        return;
    }
    // std::ios::out (default) mode creates new file, if it already exists , trunactes it
    std::ofstream truncated_file(file_path, std::ios::out);

}

void rmCmd(const CommandNode& command) {
    if(command.args.size() != 1) {
        std::cerr << "Command " << command.name << "takes 1 argument and has no options" << std::endl;
    }
    std::string file_path = command.args[0];

    // returns false if the file didn't exist
    if(!std::filesystem::remove(file_path)){
        std::cerr << "File " << file_path << " cannot be found." << std::endl;
    }

}

const std::unordered_map<std::string, std::function<void(const CommandNode&)>> command_map = {
    {"echo", echoCmd},
    {"prompt", promptCmd},
    {"time", timeCmd},
    {"date", dateCmd},
    {"touch", touchCmd},
    {"truncate", truncateCmd},
    {"rm", rmCmd},

};
