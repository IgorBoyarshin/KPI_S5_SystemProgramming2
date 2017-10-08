#include "MooreMachine.h"
#include "State.h"
#include "Signal.h"
#include "Transition.h"


const State MooreMachine::END_STATE =
    static_cast<State>(StatesAmount - 1);


MooreMachine::MooreMachine() : MooreMachine(State0) {}


MooreMachine::MooreMachine(State startState) : currentState(startState) {
    // Fill the transitionMatrix with default transitions
    for (unsigned int stateIndex = State0; stateIndex < StatesAmount; stateIndex++) {
        const State state = static_cast<State>(stateIndex);
        const State nextState = (stateIndex == State8)
            ? State8 : static_cast<State>(stateIndex + 1);

        for (unsigned int signalIndex = SignalDlm; signalIndex < SignalsAmount; signalIndex++) {
            const Signal signal = static_cast<Signal>(signalIndex);

            transitionMatrix[state][signal] = nextState;
        }
    }
}


MooreMachine::MooreMachine(
        const std::vector<Transition>& additionalTransitions)
    : MooreMachine(State0, additionalTransitions) {}


MooreMachine::MooreMachine(
        State state, const std::vector<Transition>& additionalTransitions)
    : MooreMachine(state) {
    // Update the transitionMatrix with additionalTransitions
    for (const Transition& transition : additionalTransitions) {
        const State from    = transition.from;
        const State to      = transition.to;
        const Signal signal = transition.signal;

        transitionMatrix[from][signal] = to;
    }
}


void MooreMachine::performStep(Signal signal) {
    std::cout << ":> Signal: " << signal << " | ";

    if (currentState == MooreMachine::END_STATE) {
        std::cout << "Already at the end state!" << std::endl;
    } else {
        const State newState = transitionMatrix[currentState][signal];
        std::cout << "From: " << currentState << " | "
                  << "To: " << newState
                  << std::endl;

        currentState = newState;
    }
}


void MooreMachine::performSteps(const std::vector<Signal>& signals) {
    for (const Signal& signal : signals) {
        performStep(signal);
    }
}
