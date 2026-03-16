#pragma once
#include "Executor.h"
#include <windows.h>

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

            // Don't go to new line if it already there. 
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi) && csbi.dwCursorPosition.X != 0) {
                std::cout << std::endl;
            }
            
        }
    }

    void setPromptSymbol(std::string new_symbol) {
        prompt_symbol = new_symbol;
    }

private:
    Console() = default;

    std::string prompt_symbol = "$";

    
};

