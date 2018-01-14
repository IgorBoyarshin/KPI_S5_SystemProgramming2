#include <iostream>
#include <fstream>
#include <vector>
#include "Token.h"
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"


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
    // std::cout << la << std::endl;
    SyntaxAnalyzer sa;
    const bool inputCorrect = sa.parse(tokens);
    std::cout << (inputCorrect ? "correct" : "incorrect") << std::endl;

    std::cout << "---------- Result ---------" << std::endl;
    // for (const auto& v : sa.getNodes()) {
    //     std::cout << v << std::endl;
    // }


    // CodeGenerator cg(sa.getRootNode());
    // const auto code = cg.generate();
    // std::cout << "---------- Code Generation ---------" << std::endl;
    // if (!cg.errorWasFound()) {
    //     std::cout << code;
    // }

    return 0;
}
