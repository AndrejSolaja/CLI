#include <iostream>
#include "Executor.h"

int main()
{

    std::string input;

    while(true){
        std::cout << "$ ";
        std::getline(std::cin, input);

        Tokenizer tokenizer(input);
        Parser parser(tokenizer);
        Executor executor(parser);
        executor.executeCommands();
        std::cout << std::endl;
    }


    

    
    

}

