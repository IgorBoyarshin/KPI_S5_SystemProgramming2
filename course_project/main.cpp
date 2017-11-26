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
#include "CodeGenerator.h"


int main() {
    std::cout << "---------------- Program start ----------------"
              << std::endl;

    const std::string input = "int a; int f(int d, int t){a = 4;} int main(){a = 2 + f(3, 4);}";
    // const std::string input = "int iVarA; bool bFunC(float fVarFf){} int iVarB; int main(bool bVarAaa, int iVarBbb, float fVarCcc){if (true == bFunC()) {iVarB=2*iVarA;} else iVarB= iVarA;}";
    std::cout << "Input: " << input << std::endl;
    std::cout << "---------------- Parsing ----------------"
              << std::endl;

    LexicalAnalyzer la;
    SyntaxAnalyzer sa{la.analyze(input)};
    const bool inputCorrect = sa.parse();
    std::cout << "---------- Result ---------" << std::endl;
    std::cout << "Input " << ((inputCorrect) ? "correct" : "incorrect") << std::endl;


    std::cout << "---------- Code Generation ---------" << std::endl;
    CodeGenerator cg(sa.getRootNode());
    std::cout << cg.generate();

    return 0;
}
