/*
 * System Programming
 * "Creation of Nodes for tree-like graphs with hierarchy"
 *
 * Author: Igor Boyarshin, #5207, IO-52, FIOT
 * Date: 07.10.17
 */

#include <iostream>
#include <vector>
#include "Token.h"
#include "Node.h"
#include "MooreMachine.h"
#include "Transition.h"
#include "Signal.h"
#include "State.h"


void doParseGraph();
void doMooreMachine();


int main() {
    std::cout << "---------------- Program start ----------------"
              << std::endl;

    std::cout << "---------------- Parse Graph ----------------"
              << std::endl;

    doParseGraph();

    std::cout << "---------------- Moore Machine ----------------"
              << std::endl;

    doMooreMachine();

    return 0;
}


void doParseGraph() {
    const Token tokenA("a");
    const Token tokenB("b");
    const Token tokenC("c");
    const Token token2("2");

    const Node node1(Token::LEFT_BRACKET);
    const Node node2(Token::RIGHT_BRACKET);
    const Node node3(tokenC, &node1, &node2);
    const Node node4(tokenB);
    const Node node5(Token::LEFT_BRACKET);
    const Node node6(token2, &node5, nullptr);
    const Node node7(Token::COLUMN);
    const Node node8(Token::RIGHT_BRACKET, nullptr, &node7);
    const Node node9(tokenA, nullptr, &node8);
    const Node node10(Token::MUL, &node6, &node9);
    const Node node11(Token::SIN, nullptr, &node10);
    const Node node12(Token::ASSIGN, &node4, &node11);
    const Node node13(Token::IFCOND, &node3, &node12);
    const Node node14(Token::IF, nullptr, &node13);
    const Node node15(tokenB);
    const Node node16(token2);
    const Node node17(Token::COLUMN);
    const Node node18(tokenA, nullptr, &node17);
    const Node node19(Token::MUL, &node16, &node18);
    const Node node20(Token::ASSIGN, &node15, &node19);
    const Node node21(Token::ELSE, nullptr, &node20);
    const Node node22(Token::IFBLOCK, &node14, &node21);

    std::cout << &node22 << std::endl;
}


void doMooreMachine() {
    MooreMachine mooreMachine({
        Transition(State3, State3, SignalDlm),
        Transition(State5, State1, SignalLtr)
    });

    mooreMachine.performSteps({
        SignalLtr,
        SignalDlm,
        SignalCfr,
        SignalDlm,
        SignalCfr,
        SignalCfr,
        SignalLtr,
        SignalCfr,
        SignalCfr,
        SignalCfr,
        SignalCfr,
        SignalCfr,
        SignalCfr,
        SignalCfr,
        SignalCfr
    });

    // mooreMachine.performStepsRaw(
    //         "ltr dlm cfr dlm cfr cfr ltr cfr cfr cfr cfr cfr cfr cfr cfr");
}
