#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "CodeGenerator.h"


int main() {
    std::cout << "------ Compilation start ------"
              << std::endl;

    std::string input;
    {
        std::ifstream inputFile("input.txt");
        char c;
        unsigned int amountOfSpaces = 0;
        while (inputFile.get(c)) {
            if (c == '\r') {
                continue;
            }

            if (c == ' ') {
                amountOfSpaces++;
            } else {
                if (amountOfSpaces > 0) {
                    for (unsigned int i = 0; i < amountOfSpaces / 4; i++) {
                        input += '\t';
                    }
                    for (unsigned int i = 0; i < amountOfSpaces % 4; i++) {
                        input += ' ';
                    }
                }
                amountOfSpaces = 0;

                input += c;
            }
        }
        inputFile.close();
    }

    std::cout << "Input: "
              << std::endl
              << input
              << std::endl;
    std::cout << "----- Lexical Analyzer -------"
              << std::endl;

    LexicalAnalyzer la;
    std::vector<Token> tokens = la.analyze(input);
    std::cout << "----- Syntax Analyzer -------"
              << std::endl;
    SyntaxAnalyzer sa;
    const bool inputCorrect = sa.parse(tokens);
    // std::cout << (inputCorrect ? "correct" : "incorrect") << std::endl;

    if (!inputCorrect) {
        return -1;
    }
    std::cout << "---------- Result ---------" << std::endl;

    CodeGenerator cg(sa.getNodes(), sa.getVariablesTable());
    const std::string code = cg.generate();
    std::ofstream outFile;
    outFile.open("output.asm");
    outFile << code;
    outFile.close();

    std::cout << "Writing to output.asm..."
              << std::endl;

    return 0;
}
