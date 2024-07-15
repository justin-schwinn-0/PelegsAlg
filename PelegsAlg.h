#ifndef PELEGS_ALG_H
#define PELEGS_ALG_H

#include <string>

#include "SynchAlg.h"

struct SyncMsg;

class PelegsAlg : public SynchAlg
{
public:
    PelegsAlg(Node& n);

    void handlePayload(SyncMsg payload);

    void proceedRound(int round);

private:
    int knownHighest;
    int dist;
    int distMax;

    int roundsSinceChange;

    bool mChanged;

    bool mFoundLeader;
    int leaderProp;

    const std::string FOUND_LEADER = "Found Leader!";
};

#endif
