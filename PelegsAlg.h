#ifndef PELEGS_ALG_H
#define PELEGS_ALG_H

#include <string>

#include "SynchAlg.h"

class PelegsAlg : public SynchAlg
{
public:
    PelegsAlg(Node& n);

    void handlePayload(std::string payload);

    void proceedRound(int round);

private:
    int knownHighest;
    int dist;
    int distMax;

    int roundsSinceChange;

    bool mChanged;
};

#endif
