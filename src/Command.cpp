#include "Command.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <filesystem>

#include "Console.h"
#include "Executor.h"

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
        return;
    }
    else if(command.args.size() == 1) {
        std::string arg = command.args[0];
        if(arg.size() >= 2 && arg[0] == '"' && arg[arg.size() - 1] == '"') {
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
        std::string line;
        while(std::getline(std::cin, line)){
            buffer += line;
            if(!std::cin.eof())
                buffer += '\n';
        }
    }
    
    std::cout << buffer;

}

void promptCmd(const CommandNode& command) {
    if(command.args.size() != 1){
        std::cerr << "Command " << command.name << "takes 1 argument and has no options" << std::endl;
        return;
    }
    std::string arg = command.args[0];
    if(arg[0] == '"' && arg[arg.size() - 1] == '"') {
            Console::getInstance().setPromptSymbol(arg.substr(1, arg.size() - 2));
    }else{
        std::cerr << "Command " << command.name << "argument must be enclosed in double quotes" << std::endl;
        return;
    }

}

void timeCmd(const CommandNode& command) {
    if(command.args.size() != 0) {
        std::cerr << "Command " << command.name << "takes no argument and has no options" << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    tm* local = std::localtime(&t);

    std::cout << std::put_time(local, "%H:%M:%S");

}

void dateCmd(const CommandNode& command) {
    if(command.args.size() != 0) {
        std::cerr << "Command " << command.name << "takes no argument and has no options" << std::endl;
        return;
    }

    auto now = std::chrono::system_clock::now();
    time_t t = std::chrono::system_clock::to_time_t(now);
    tm* local = std::localtime(&t);

    std::cout << std::put_time(local, "%Y-%m-%d");

}

void touchCmd(const CommandNode& command) {
    if(command.args.size() != 1) {
        std::cerr << "Command " << command.name << "takes 1 argument and has no options" << std::endl;
        return;
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
        return;
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
        return;
    }
    std::string file_path = command.args[0];

    // returns false if the file didn't exist
    if(!std::filesystem::remove(file_path)){
        std::cerr << "File " << file_path << " cannot be found." << std::endl;
        return;
    }

}

void wcCmd(const CommandNode& command) {
    if(command.args.size() != 1 && command.args.size() != 2) {
        std::cerr << "Command " << command.name << "takes at most 1 argument and 1 option" << std::endl;
        return;
    }
    std::optional<std::string> arg;
    std::optional<std::string> option;


    for(auto x : command.args){
        if(x[0] == '-') {
            if(option.has_value()){
                std::cerr << "Command " << command.name << "takes 1 option" << std::endl;
                return;
            }
            option = x;
        }else{
            arg = x;
        }
    }

    // option must be specified
    if(!option.has_value()){
        std::cerr << "Command " << command.name << "takes at most 1 argument and 1 option" << std::endl;
        return;
    }

    std::string buffer;
    std::ifstream input_stream;
    if(arg.has_value()){
        if(arg.value().size() >= 2 && arg.value()[0] == '"' && arg.value()[arg.value().size() - 1] == '"') {
            buffer = arg.value().substr(1, arg.value().size() - 2);
        }else {
            // file path
            input_stream.open(arg.value());
            if (!input_stream.is_open()) {
                std::cerr << "File " << command.args[0] << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::cin.rdbuf(input_stream.rdbuf());

            std::ostringstream ss;
            ss << input_stream.rdbuf();
            buffer = ss.str();
        }
    }else{
        std::string line;
        while(std::getline(std::cin, line)){
            buffer += line;
            if(!std::cin.eof())
                buffer += '\n';
        }
    }

    int count = 0;

    if(option.value() == "-w"){
        std::istringstream iss(buffer);
        std::string word;
        while(iss >> word) count++;
    }else if (option.value() == "-c"){
        count = buffer.size();
    }

    std::cout << count;

}

void trCmd(const CommandNode& command) {
    if(command.args.size() < 1 || command.args.size() > 3) {
        std::cerr << "Command " << command.name << " takes 1 required option and optional argument and replacement" << std::endl;
        return;
    }

    std::optional<std::string> input_arg;
    std::optional<std::string> what;
    std::string with = "";

    for(const auto& x : command.args) {
        if(x[0] == '-') {
            std::string w = x.substr(1);
            if(w.size() >= 2 && w[0] == '"' && w[w.size()-1] == '"')
                what = w.substr(1, w.size() - 2);
            else {
                std::cerr << "Invalid input for what option, needs to be enclosed in quotes" << std::endl;
                return;
            }
        } else if(!input_arg.has_value()) {
            // First quoted non - argument
            input_arg = x;
        } else {
            // Second quoted non - argument
            std::string w = x;
            if(w.size() >= 2 && w[0] == '"' && w[w.size()-1] == '"')
                with = w.substr(1, w.size() - 2);
            else {
                std::cerr << "Invalid input for with option, needs to be enclosed in quotes" << std::endl;
                return;
            }
        }
    }

    if(!what.has_value()) {
        std::cerr << "Command " << command.name << " requires a -\"what\" option" << std::endl;
        return;
    }

    std::string buffer;
    std::ifstream input_stream;
    if(input_arg.has_value()) {
        const std::string& arg = input_arg.value();
        if(arg.size() >= 2 && arg[0] == '"' && arg[arg.size()-1] == '"') {
            buffer = arg.substr(1, arg.size() - 2);
        } else {
            input_stream.open(arg);
            if(!input_stream.is_open()) {
                std::cerr << "File " << arg << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::ostringstream ss;
            ss << input_stream.rdbuf();
            buffer = ss.str();
        }
    } else {
        std::string line;
        while(std::getline(std::cin, line)) {
            buffer += line;
            if(!std::cin.eof())
                buffer += '\n';
        }
    }

    // replace all occurrences of 'what' with 'with' (or remove if 'with' not set)
    const std::string& search = what.value();
    const std::string replacement = with;
    std::string result;
    size_t pos = 0, found;
    while((found = buffer.find(search, pos)) != std::string::npos) {
        result += buffer.substr(pos, found - pos);
        result += replacement;
        pos = found + search.size();
    }
    result += buffer.substr(pos);
    
    std::cout << result;
}

void headCmd(const CommandNode& command) {
    if(command.args.size() < 1 || command.args.size() > 2) {
        std::cerr << "Command " << command.name << " takes 1 required option and optional argument " << std::endl;
        return;
    }

    std::optional<std::string> input_arg;
    std::optional<std::string> n_count;

    for(const auto& x : command.args) {
        if(x[0] == '-') {
            n_count = x.substr(1);
        } else {
            input_arg = x;
        }
    }

    if(!n_count.has_value()){
        std::cerr << "Command " << command.name << " requires a -\"ncount\" option" << std::endl;
        return;
    }

    std::string buffer;
    std::ifstream input_stream;
    if(input_arg.has_value()) {
        const std::string& arg = input_arg.value();
        if(arg.size() >= 2 && arg[0] == '"' && arg[arg.size()-1] == '"') {
            buffer = arg.substr(1, arg.size() - 2);
        } else {
            input_stream.open(arg);
            if(!input_stream.is_open()) {
                std::cerr << "File " << arg << " does not exist or cannot be opened." << std::endl;
                return;
            }
            std::ostringstream ss;
            ss << input_stream.rdbuf();
            buffer = ss.str();
        }
    } else {
        std::string line;
        while(std::getline(std::cin, line)) {
            buffer += line;
            if(!std::cin.eof())
                buffer += '\n';
        }
    }

    int ncount = std::stoi(n_count.value().substr(1));  // n3 -> 3
    std::istringstream iss(buffer);
    std::string line;
    for(int i = 0; i < ncount && std::getline(iss, line); i++)
        std::cout << line << '\n';

}

void batchCmd(const CommandNode& command) {
    if(command.args.size() != 1) {
        std::cerr << "Command " << command.name << " takes 1 required argument " << std::endl;
    }

    std::string file_path = command.args[0];

    std::ifstream existing_file(file_path);
    if (!existing_file.is_open()) {
        std::cerr << "File " << file_path << " cannot be found." << std::endl;
        return;
    }

    std::ifstream batch_file(file_path);
    std::string line;
    while(std::getline(batch_file, line)){
        Tokenizer tokenizer(line);
        Parser parser(tokenizer);
        Executor executor(parser);
        executor.executeCommands();
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
    {"wc", wcCmd},
    {"tr", trCmd},
    {"head", headCmd},
    {"batch", batchCmd}

};
