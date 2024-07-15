#include "PelegsAlg.h"

#include "Node.h"
#include "Utils.h"
#include<unistd.h>


#include <algorithm>

PelegsAlg::PelegsAlg(Node& n) : 
    SynchAlg(n),
    knownHighest(n.getUid()),
    dist(0),
    distMax(0),
    roundsSinceChange(0),
    mChanged(false),
    mFoundLeader(false)
{}

void PelegsAlg::handlePayload(SyncMsg msg)
{ 
    std::string payload = msg.payload;
    int uid = msg.uid;

    if(payload == FOUND_LEADER || mFoundLeader) 
    {
    	mFoundLeader = true;
	leaderProp = uid;
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
    if(mFoundLeader)
    {
        Utils::log("Leader is", knownHighest);
        rNode.sendExcept(leaderProp,SynchAlg::wrapPayload(FOUND_LEADER,round));
        rNode.setIsLeader(knownHighest);
        rNode.finishAlg();
	sleep(2);
	return;
    }
    else if(mChanged)
    {   
        roundsSinceChange = 0;
    }
    else
    {
        roundsSinceChange++;
    }
    mChanged = false;
    
    if(roundsSinceChange >= 3 && knownHighest == rNode.getUid())
    {
        Utils::log("================== Leader elected:", knownHighest);
        rNode.flood(SynchAlg::wrapPayload(FOUND_LEADER,round));
	mFoundLeader = true;
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
