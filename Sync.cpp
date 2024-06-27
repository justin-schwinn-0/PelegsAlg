#include "Sync.h"
#include "Node.h"
#include "Utils.h"

Sync::Sync(int neighbors) : 
    mRound(0), 
    affixedVectors(false)
{
}

void Sync::msgHandler(std::string s)
{

    Utils::log("got here", std::to_string(__LINE__));
    std::string payload = parseMsg(s);

    mHandlePayload(payload);

    progressRound();
}

std::string Sync::parseMsg(std::string r)
{
// uid::round::payload

    auto segments = Utils::split(r,"::");
    Utils::log("got here", std::to_string(__LINE__));

    int uid = Utils::strToInt(segments[0]); 
    int round = Utils::strToInt(segments[1]); 

    auto it = mHasRecvd.find(uid);
    if(it != mHasRecvd.end())
    {   
        it->second = true;
    }
    else
    {
        mHasRecvd[uid] = true;
    }

    //Utils::printVectorPair(mHasRecvd);
    for(auto& pair : mHasRecvd)
    {
        if(pair.first == uid)
        {
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
    Utils::log("got here", std::to_string(__LINE__));
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
        //Utils::log("sent next round!");
        mProceedRound(mRound);
    }
    else
    {
        //Utils::log("cannot progress yet");
        //Utils::printVectorPair(mHasRecvd);
    }
    Utils::log("got here", std::to_string(__LINE__));
}

void Sync::init()
{
    mProceedRound(0);
}
