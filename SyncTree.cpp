#include "SyncTree.h"
#include "Node.h"
#include "Utils.h"

SyncTree::SyncTree(int neighbors) : 
    mKnownNeighbors(neighbors)
{
}

void SyncTree::msgHandler(std::string s)
{

    auto data =  Utils::split(s,"--");

    if(data[0] == "par") // if recived parent
    {
        int pid = Utils::strToInt(data[1]);
        int lid = Utils::strToInt(data[2]);
        parentUid = pid;
        leaderUid = lid;

        Utils::log("found parent and leader",parentUid,leaderUid);
    }
    else if( data[0] == "child") // ack parent
    {
        
    }
    else if(data[0] == "ref") // refuse parent
    {

    }
}

std::string SyncTree::parseMsg(std::string r)
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

void SyncTree::progressRound()
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

void SyncTree::init()
{
    mProceedRound(mRound);
}
