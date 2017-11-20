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

    const std::string input = "if (a != 2) {c(3, 5);} else b= ;";
    std::cout << "Input: " << input << std::endl;

    LexicalAnalyzer la;
    SyntaxAnalyzer sa{la.analyze(input)};
    // SyntaxAnalyzer sa{la.analyze("if (a != 2) b = c;")};
    const bool inputCorrect = sa.parse();
    std::cout << "---------- Result ---------" << std::endl;
    std::cout << "Input " << ((inputCorrect) ? "correct" : "incorrect") << std::endl;

    return 0;
}
