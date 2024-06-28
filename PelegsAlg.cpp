#include "PelegsAlg.h"

#include "Node.h"
#include "Utils.h"

#include <algorithm>

PelegsAlg::PelegsAlg(Node& n) : 
    SynchAlg(n),
    knownHighest(n.getUid()),
    dist(0),
    distMax(0),
    roundsSinceChange(0),
    mChanged(false)
{
}

void PelegsAlg::handlePayload(std::string payload)
{

    if(payload == FOUND_LEADER) 
    {
        Utils::log("Leader is", knownHighest);
        rNode.setIsLeader(knownHighest);
        rNode.finishAlg();
        return;
    }

    auto triplet = Utils::split(payload,"~~");

    int otherHighest = Utils::strToInt(triplet[0]);
    int otherDist = Utils::strToInt(triplet[1]);
    int otherDistMax = Utils::strToInt(triplet[2]);


    if(otherHighest > knownHighest)
    {
        knownHighest = otherHighest;
        dist = otherDist+1;
        distMax = std::max(otherDistMax,dist);
        mChanged = true;
    }
    else if( otherHighest == knownHighest)
    {
        int newDistMax = std::max(distMax,otherDistMax);
        if(newDistMax != distMax)
        {
            distMax = newDistMax;
            mChanged = true;
        }
    }

}

void PelegsAlg::proceedRound(int round)
{
    //Utils::log("current state:",knownHighest,dist,distMax);
    if(mChanged)
    {   
        roundsSinceChange = 0;
    }
    else
    {
        roundsSinceChange++;
    }
    mChanged = false;
    
    if(roundsSinceChange >= 3)
    {
        Utils::log("================== Leader elected:", knownHighest);
        rNode.flood(SynchAlg::wrapPayload(FOUND_LEADER,round));
    }
    else
    {
        // send payload with format
        // knownHighest~~dist~~distMax
        Utils::log("========= NEXT ROUND =========", round);
        std::string msg = std::to_string(knownHighest) +"~~"
                    +std::to_string(dist) + "~~" + std::to_string(distMax);
        rNode.flood(SynchAlg::wrapPayload(msg,round));
    }
}
