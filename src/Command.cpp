#include "Command.h"
#include <iostream>
#include <fstream>
#include <sstream>

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

const std::unordered_map<std::string, std::function<void(const CommandNode&)>> command_map = {
    {"echo", echoCmd},
};
