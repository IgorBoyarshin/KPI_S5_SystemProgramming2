#ifndef MOOREMACHINE_H
#define MOOREMACHINE_H

#include "State.h"
#include "Signal.h"
#include "Transition.h"
#include <vector>
#include <map>


typedef std::map<State, std::map<Signal, State> > TransitionMatrix;

class MooreMachine {
    public:
        MooreMachine();
        MooreMachine(State startState);
        MooreMachine(const std::vector<Transition>& additionalTransitions);
        MooreMachine(State startState,
                const std::vector<Transition>& additionalTransitions);

    private:
        TransitionMatrix transitionMatrix;
        State currentState;
        const static State END_STATE;

    public:
        void performStep(Signal signal);
        void performSteps(const std::vector<Signal>& signals);
};

#endif
