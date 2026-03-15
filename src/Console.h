#pragma once
#include "Executor.h"

class Console
{
public:
    static Console& getInstance()
    {
        static Console instance;
        return instance;
    }

    Console(const Console&) = delete;
    Console& operator=(const Console&) = delete;

    void run(){
        std::string input;

        while(true){
            std::cout << prompt_symbol <<" ";
            std::getline(std::cin, input);

            Tokenizer tokenizer(input);
            Parser parser(tokenizer);
            Executor executor(parser);
            executor.executeCommands();
            std::cout << std::endl;
        }
    }

    void setPromptSymbol(std::string new_symbol) {
        prompt_symbol = new_symbol;
    }

private:
    Console() = default;

    std::string prompt_symbol = "$";

    
};

