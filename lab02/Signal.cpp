#include "Signal.h"
#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& os, Signal signal) {
    switch(signal) {
        case SignalDlm:
            os << "dlm";
            break;
        case SignalCfr:
            os << "cfr";
            break;
        case SignalLtr:
            os << "ltr";
            break;
        default:
            break;
    }

    return os;
}


Signal getSignalFromString(const std::string& str) {
    if (str.compare("dlm") == 0) return SignalDlm;
    if (str.compare("cfr") == 0) return SignalCfr;
    if (str.compare("ltr") == 0) return SignalLtr;

    std::cout << "Invalid input argument for getSignalFromString(string)!"
                << std::endl
                << "Returning SignalDlm" << std::endl;
    return SignalDlm;
}
