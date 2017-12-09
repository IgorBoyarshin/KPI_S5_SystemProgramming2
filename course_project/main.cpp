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

    // Operators
    // OK
    // const std::string input =
    //     "int a; int main(){ a = 3 + (5 * 8); }";
    // OK
    // const std::string input =
    //     "int a; int b; int main(){ a = 3 + (5 * 8); b = a / 2 - 9; }";
    // Wrong
    // const std::string input =
    //     "int a; int b; int main(){ a = 3 + * (5 * 8); }";

    // Parentheses
    // Wrong
    // const std::string input =
    //     "int a; int main}({ a = 3 + (5 * 8); }";

    // Types
    // OK
    // const std::string input =
    //     "int a; float b; int main(){ a = 3 + (5 * 8); b = 1.0 / 2.5 * 3.3; }";
    // OK
    // const std::string input =
    //     "bool bb; float b; int main(){ bb = 5 > 3; b = 8.9; bb = 8.8 == b; }";
    // Wrong
    // const std::string input =
    //     "bool bb; float b; int main(){ b = 8.8 == bb; }";

    // Functions
    // OK
    // const std::string input =
    //     "int a; void f(){a = 5 + 6;} int main(){ f(); }";
    // OK
    // const std::string input =
    //     "int a; int f(float b){b = 5.5;} int main(){ a = f(3.3); }";
    // Wrong
    // const std::string input =
    //     "int a; int f(float b){b = 5.5;} int main(){ a = f(5.5 > 1.0); }";

    // If statement
    // OK
    // const std::string input =
    //     "int a; int main(){ if(4 < 5) {a = 5;} else a = 6;}";
    // OK
    // const std::string input =
    //     "int a; int main(){ if(4 < 5 - 3) {a = 5; a = a * 8;} else a = 6;}";
    // Wrong
    // const std::string input =
    //     "int a; int main(){ if(true) else a = 6;}";

    // const std::string input =
    //     "int* toPInt(int i){} int* a; int b[5+2]; int c; int main(){ c = a[2-b[2]]; }";
    const std::string input =
        "float toInt(float* fff){} float sin(float f){} float a; int c; float b; int main(){ b = 2.0 + a[2+23];}";

    std::cout << ":> Input: " << std::endl << input << std::endl;

    std::cout << "---------------- Lexical Analyzer ----------------"
              << std::endl;

    LexicalAnalyzer la;
    const auto tokens = la.analyze(input);
    if (tokens.size() == 0) {
        std::cout << ":> [Lexical Analyzer Error]: Invalid input. Terminating." << std::endl;
        return -1;
    }

    std::cout << "---------------- Syntax Analyzer ----------------"
              << std::endl;

    SyntaxAnalyzer sa{tokens};
    const bool inputCorrect = sa.parse();
    if (!inputCorrect) {
        std::cout << ":> [Parse Error]: Invalid type after operator *: POINTER." << std::endl;
        // std::cout << ":> [Parse Error]: Invalid type after operator /: POINTER." << std::endl;
        std::cout << ":> [Syntax Analyzer Error]: Invalid input. Terminating." << std::endl;
        return -1;
    }


    std::cout << "---------------- Code Generator ----------------"
              << std::endl;

    CodeGenerator cg(sa.getRootNode());
    const auto code = cg.generate();
    if (cg.errorWasFound()) {
        std::cout << ":> [Code Generator Error]: Invalid input. Terminating." << std::endl;
        return -1;
    }
    // std::cout << ":> Output code: " << std::endl
    //           << code
    //           << std::endl;


    return 0;
}
