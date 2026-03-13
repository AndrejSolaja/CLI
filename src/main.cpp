#include <iostream>
#include <fstream>

#include "Tokenizer.h"
#include "Parser.h"
#include "Executor.h"

int main()
{
    Tokenizer tokenizer("command.txt");
    Parser parser(tokenizer);
    Executor executor(parser);

    executor.executeCommands();
    

}

