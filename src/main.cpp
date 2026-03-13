#include <iostream>
#include <fstream>
#include "Tokenizer.h"
#include "Parser.h"

int main()
{
    Tokenizer tokenizer("input.txt"); 

    std::vector<Token> token_list = tokenizer.tokenize();

    for(const auto& x : token_list) {
        std::cout << tokenizer.tokenString(x) << std::endl;
    }

    Parser parser(tokenizer);

    auto commands = parser.parseInput();
    std::cout << "test";



    

}

