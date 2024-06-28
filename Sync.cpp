#include "Sync.h"
#include "Node.h"
#include "Utils.h"

Sync::Sync(int neighbors) : 
    mRound(0), 
    mKnownNeighbors(neighbors)
{
}

void Sync::msgHandler(std::string s)
{

    std::string payload = parseMsg(s);

    if(!payload.empty())
    {
        mHandlePayload(payload);

        progressRound();
    }
}

std::string Sync::parseMsg(std::string r)
{
// uid::round::payload

    auto segments = Utils::split(r,"::");

    int uid = Utils::strToInt(segments[0]); 
    int round = Utils::strToInt(segments[1]); 
    if(mRound == round+1)
    {   
        Utils::log("msg round is wrong, caching wrapped msg", round);
        Utils::log(r);
        payloadCache[uid] = r;
        return "";
    }

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


    return segments[2];
}

void Sync::progressRound()
{

    bool canProgress = true;

    if(mHasRecvd.size() != mKnownNeighbors)
    {
        return;
    }

    for(auto& pair : mHasRecvd)
    {
        if(!pair.second)
        {
            canProgress = false;
        }
    }

    if(canProgress)
    {
        Utils::log("progressing round", mRound, "->",mRound+1);
        mRound++;

        for(auto& pair : mHasRecvd)
        {
            pair.second = false;
        }
        //Utils::log("sent next round!");
        mProceedRound(mRound);

        //process all cached msgs
        
        for(auto pair : payloadCache)
        {
            msgHandler(pair.second);
        }
    }
}

void Sync::init()
{
    mProceedRound(mRound);
}
