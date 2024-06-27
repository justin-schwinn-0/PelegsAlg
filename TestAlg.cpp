#include "TestAlg.h"

#include "Node.h"
#include "Utils.h"

TestAlg::TestAlg(Node& n) : 
    SynchAlg(n)
{
}

void TestAlg::handlePayload(std::string payload)
{
    Utils::log(payload);
}

void TestAlg::proceedRound(int round)
{
    Utils::log("========= NEXT ROUND =========", round);
    std::string msg = std::to_string(rNode.getUid())+" at round " + std::to_string(round);
    rNode.flood(SynchAlg::wrapPayload(msg,round));
}
