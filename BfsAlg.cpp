#include "BfsAlg.h"

#include "Node.h"
#include "Utils.h"
BfsAlg::BfsAlg(Node& n) : 
    SynchAlg(n),
    parentUid(1),
    mClaimChildren(false)
{
}

void BfsAlg::handlePayload(std::string payload)
{
    Utils::log("got", payload);
    auto data = Utils::split(payload,"||");

    int uid = Utils::strToOInt(data[0]);


    if(parentUid == -1)
    {
        parentUid = uid;
        mClaimChildren = true;
    }

}

void BfsAlg::proceedRound(int round)
{
    if(mClaimChildren)
    {
        mClaimChildren = false;
        rNode.sendExcept(parentUid,wrapPayload(msg,round));
    }
}

std::string BfsAlg::parentStr()
{
    return rNode.getUid() + "||parent";
}

void BfsAlg::rootTree()
{
    rNode.flood(wrapPayload(msg,0));
}
