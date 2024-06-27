#include "Sync.h"
#include "Node.h"
#include "Utils.h"

void Sync::msgHandler(std::string s)
{

    std::string payload = parseMsg(s);

}

std::string parseMsg(std::string r)
{
// uid::round::payload

    auto segments = Utils::split(r,"::");

    int uid = Utils::strToInt(segments[0]); 
    int round = Utils::strToInt(segments[1]); 

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

void progressRound()
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
        rNode.flood(wrapPayload("test"));
    }
}

std::string wrapPayload(std::string payload)
{
    return std::to_string(rNode.getUid) + "::" + std::to_string(mRound) + "::" + payload;
}
