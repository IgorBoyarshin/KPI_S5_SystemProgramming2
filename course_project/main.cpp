/*
 * System Programming
 * "Syntax Analyzer"
 *
 * Author: Igor Boyarshin, #5207, IO-52, FIOT
 * Date: 19.11.17
 */

#include <iostream>
#include <vector>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Token.h"


int main() {
    std::cout << "---------------- Program start ----------------"
              << std::endl;

    const std::string input = "int a; int c(){} int b; int main(){if (a >= c()) {b=2*a;} else b= a;}";
    std::cout << "Input: " << input << std::endl;
    std::cout << "---------------- Parsing ----------------"
              << std::endl;

    LexicalAnalyzer la;
    SyntaxAnalyzer sa{la.analyze(input)};
    const bool inputCorrect = sa.parse();
    std::cout << "---------- Result ---------" << std::endl;
    std::cout << "Input " << ((inputCorrect) ? "correct" : "incorrect") << std::endl;

    return 0;
}
