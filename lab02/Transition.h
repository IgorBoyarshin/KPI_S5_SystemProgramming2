#ifndef TRANSITION_H
#define TRANSITION_H

#include "State.h"
#include "Signal.h"

struct Transition {
    Transition(State from, State to, Signal signal)
        : from(from), to(to), signal(signal) {}

    const State from;
    const State to;
    const Signal signal;
};

#endif
