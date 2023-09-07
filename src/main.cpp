#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Generator.hpp"

int main(int argc, char** argv) {
    if (argv[1] == NULL){
        std::cout << "Incorrect usage. Please use the following format: ./a.out <filename>" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string output;
    {
        std::stringstream buffer;
        std::ifstream file(argv[1]);
        buffer << file.rdbuf();
        output = buffer.str();
    }

    Tokenizer tokenizer(output);
    std::vector<Token> tokens = tokenizer.tokenize();
    Parser parser(tokens);
    std::optional<Node> nodes = parser.parse();
    Generator generator(nodes.value());
    std::string generated_code = generator.generate();
    {
        std::ofstream file("output.cpp");
        file << generated_code;
    }
    system("g++ output.cpp -o out");
    return 0;
}