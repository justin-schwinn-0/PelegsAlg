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
    Utils::log("got", payload);

    auto triplet = Utils::split(payload,"~~");

    int otherHighest = Utils::strToInt(triplet[0]);
    int otherDist = Utils::strToInt(triplet[1]);
    int otherDistMax = Utils::strToInt(triplet[2]);

    bool changed = false;

    if(otherHighest > knownHighest)
    {
        knownHighest = otherHighest;
        dist = otherDist+1;
        distMax = std::max(otherDistMax,dist);
        changed = true;
    }
    else if( otherHighest == knownHighest)
    {
        int newDistMax = std::max(distMax,otherDistMax);
        if(newDistMax != distMax)
        {
            distMax = newDistMax;
            changed = true;
        }
    }
    
    if(roundsSinceChange == 3)
    {
        Utils::log("================== Leader elected:", knownHighest);
        exit(1);
    }

    if(changed)
    {   
        roundsSinceChange = 0;
    }
    else
    {
        roundSinceChange++;
        Utils::log("rounds since change:",roundsSinceChange);
    }

}

void PelegsAlg::proceedRound(int round)
{
    Utils::log("current state:",knownHighest,dist,distMax,round);
    // send payload with format
    // knownHighest~~dist~~distMax
    Utils::log("========= NEXT ROUND =========", round);
    std::string msg = std::to_string(knownHighest) +"~~"
                +std::to_string(dist) + "~~" + std::to_string(distMax);
    rNode.flood(SynchAlg::wrapPayload(msg,round));
}
