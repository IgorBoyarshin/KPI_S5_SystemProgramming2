#ifndef SIGNAL_H
#define SIGNAL_H

#include <iostream>
#include <string>

enum Signal {
    SignalDlm,
    SignalCfr,
    SignalLtr,
    SignalsAmount
};


std::ostream& operator<<(std::ostream& os, const Signal& signal);
Signal getSignalFromString(const std::string& str);

#endif
