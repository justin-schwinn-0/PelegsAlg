#include "PelegsAlg.h"

#include "Node.h"
#include "Utils.h"

#include <algorithm>

PelegsAlg::PelegsAlg(Node& n) : 
    SynchAlg(n),
    knownHighest(n.getUid()),
    dist(0),
    distMax(0),
    roundsSinceChange(0)
{
    Utils::log("made alg");
}

void PelegsAlg::handlePayload(std::string payload)
{

    auto triplet = Utils::split(payload,"~~");

    int otherHighest = Utils::strToInt(triplet[0]);
    int otherDist = Utils::strToInt(triplet[1]);
    int otherDistMax = Utils::strToInt(triplet[2]);

    Utils::log("compare",knownHighest,otherHighest);

    if(otherHighest > knownHighest)
    {
        knownHighest = otherHighest;
        dist = otherDist+1;
        distMax = std::max(otherDistMax,dist);
        roundsSinceChange = 0;
    }
    else if( otherHighest == knownHighest)
    {
        distMax = std::max(distMax,otherDistMax);
        roundsSinceChange = 0;
    }
    else
    {
        roundsSinceChange++;
    }

    if(roundsSinceChange == 3)
    {
        Utils::log("Leader elected:", knownHighest);

    }

}

void PelegsAlg::proceedRound(int round)
{
    Utils::log("current state:",knownHighest,dist,distMax);
    // send payload with format
    // knownHighest~~dist~~distMax
    Utils::log("========= NEXT ROUND =========", round);
    std::string msg = std::to_string(knownHighest) +"~~"
                +std::to_string(dist) + "~~" + std::to_string(distMax);
    rNode.flood(SynchAlg::wrapPayload(msg,round));
}
