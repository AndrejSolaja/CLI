#include <iostream>
#include <fstream>
#include "Tokenizer.h"

int main()
{
    Tokenizer tokenizer("input.txt"); 
    std::vector<token_t> token_list = tokenizer.tokenize();

    for(const auto& x : token_list) {
        std::cout << tokenizer.token_string(x) << std::endl;
    }

}

