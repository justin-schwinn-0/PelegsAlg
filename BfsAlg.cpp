#include "BfsAlg.h"

#include "Node.h"
#include "Utils.h"
BfsAlg::BfsAlg(Node& n) : 
    SynchAlg(n),
    parentUid(1),
    mClaimChildren(false),
    mLeader(-1)
{
}

void BfsAlg::handlePayload(std::string payload)
{
    auto data = Utils::split(payload,"||");

    if(data[1] == "parent")
    {
        if(!rNode.isLeader() && parentUid == -1)
        {
            int uid = Utils::strToInt(data[0]);
            int lead = Utils::strToInt(data[2]);
            parentUid = uid;
            mLeader = lead;
            rNode.sendExcept(parentUid,wrapPayload(parentStr(),0));
        }
    }
    else if(data[1] == "child")
    {

    }
    else if(data[1] == "refuse")
    {
        
    }


}

void BfsAlg::proceedRound(int round)
{
    Utils::log("========= NEXT ROUND =========", round);
    if(round == 0 && rNode.isLeader())
    {
        rootTree();
    }
    else if(mClaimChildren)
    {
        mClaimChildren = false;
    }
}

std::string BfsAlg::parentStr()
{
    return std::to_string(rNode.getUid()) + "||parent||" + std::to_string(mLeader);
}

std::string BfsAlg::amChild()
{
    return std::to_string(rNode.getUid()) + "||child||" + std::to_string(mLeader);
}

void BfsAlg::rootTree()
{
    Utils::log("i am the leader!");
    rNode.flood(wrapPayload(parentStr(),0));
}
