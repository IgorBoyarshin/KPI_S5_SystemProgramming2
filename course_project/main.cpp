/*
 * System Programming
 * "Compiler from C to x86 Assembly"
 *
 * Author: Igor Boyarshin, #5207, IO-52, FIOT
 * Date: 30.11.17
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

    const std::string input = "int a; void ff(bool bb){a=9;} int f(bool rr, int d, float t){ff(3.2 == 3.3 + 1.1);} int main(){if (3>2) {a=2;} else a=3; a = 2 + f(5 > 6, 8, 5.1);}";
    // const std::string input = "int iVarA; bool bFunC(float fVarFf){} int iVarB; int main(bool bVarAaa, int iVarBbb, float fVarCcc){if (true == bFunC()) {iVarB=2*iVarA;} else iVarB= iVarA;}";
    std::cout << "Input: " << input << std::endl;
    std::cout << "---------------- Parsing ----------------"
              << std::endl;

    LexicalAnalyzer la;
    SyntaxAnalyzer sa{la.analyze(input)};
    const bool inputCorrect = sa.parse();
    std::cout << "---------- Result ---------" << std::endl;
    std::cout << "Input " << ((inputCorrect) ? "correct" : "incorrect") << std::endl;


    CodeGenerator cg(sa.getRootNode());
    const auto code = cg.generate();
    std::cout << "---------- Code Generation ---------" << std::endl;
    if (!cg.errorWasFound()) {
        std::cout << code;
    }

    return 0;
}
