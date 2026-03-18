#include "Console.h"
#include <iostream>
#include <windows.h>
#include "Tokenizer.h"
#include "Parser.h"
#include "Executor.h"

Console& Console::getInstance()
{
    static Console instance;
    return instance;
}

void Console::run()
{
    std::string input;

    while (true) {
        std::cout << prompt_symbol << " ";
        std::getline(std::cin, input);

        Tokenizer tokenizer(input);
        Parser parser(tokenizer.tokenize());
        Executor executor(parser.parse());
        executor.executeCommands();

        // Don't go to new line if it already there.
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi) && csbi.dwCursorPosition.X != 0) {
            std::cout << std::endl;
        }
    }
}

void Console::setPromptSymbol(std::string new_symbol)
{
    prompt_symbol = new_symbol;
}
