#include "Sync.h"
#include "Node.h"
#include "Utils.h"

Sync::Sync(int neighbors, Node& n) : 
    rNode(n), 
    mRound(0), 
    affixedVectors(false)
{
}

void Sync::msgHandler(std::string s)
{

    std::string payload = parseMsg(s);

    progressRound();
}

std::string Sync::parseMsg(std::string r)
{
// uid::round::payload

    auto segments = Utils::split(r,"::");

    int uid = Utils::strToInt(segments[0]); 
    int round = Utils::strToInt(segments[1]); 

    auto it = mHasRecvd.find(uid);
    if(it != mHasRecvd.end())
    {   
        it.second = true;
    }
    else
    {
        mHasRecvd[uid] = true;
    }

    for(auto& pair : mHasRecvd)
    {
        if(pair.first == uid)
        {
            Utils::log("recvd from " , uid);
            break;
        }
    }

    if(mRound == round)
    {   
        Utils::log(uid," is in rnd", mRound, "like me!");
    }
    else
    {
        Utils::log(uid," is in rnd", round, "unlike me in rnd",mRound);
    }

    return segments[2];
}

void Sync::progressRound()
{
    bool canProgress = true;

    for(auto& pair : mHasRecvd)
    {
        if(!pair.second)
        {
            canProgress = false;
        }
    }

    if(canProgress)
    {
        mRound++;

        for(auto& pair : mHasRecvd)
        {
            pair.second = false;
        }
        Utils::log("sent next round!");
        rNode.flood(wrapPayload("test"));
    }
    else
    {
        Utils::log("cannot progress yet");
        Utils::printVectorPair(mHasRecvd);
    }
}

std::string Sync::wrapPayload(std::string payload)
{
    return std::to_string(rNode.getUid()) + "::" + std::to_string(mRound) + "::" + payload;
}

void Sync::init()
{
    rNode.flood(wrapPayload("test"));
}
