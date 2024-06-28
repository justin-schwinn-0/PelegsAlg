#ifndef PELEGS_ALG_H
#define PELEGS_ALG_H

#include <string>

#include "SynchAlg.h"

class PelegsAlg : public SynchAlg
{
public:
    PelegsAlg(Node& n);

    void handlePayload(int uid,std::string payload);

    void proceedRound(int round);

private:
    int knownHighest;
    int dist;
    int distMax;

    int roundsSinceChange;

    bool mChanged;

    bool mLeaderFound;

    const std::string FOUND_LEADER = "Found Leader!";
    const std::string ID_PAR = "id parent";
    const std::string ID_CHILD = "id child";
};

#endif
