/*
 * System Programming
 * "Lexical Analyzer"
 *
 * Author: Igor Boyarshin, #5207, IO-52, FIOT
 * Date: 22.10.17
 */

#include <iostream>
#include <vector>
#include "LexicalAnalyzer.h"
#include "Token.h"


int main() {
    std::cout << "---------------- Program start ----------------"
              << std::endl;

    LexicalAnalyzer la;
    la.analyze("if(c) b=sin(2*a); else b=2*a;");
    std::cout << la << std::endl;

    return 0;
}
