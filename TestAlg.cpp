#include "TestAlg.h"

#include "Node.h"
#include "Utils.h"

TestAlg::TestAlg(Node& n) : 
    SynchAlg(n)
{
}

void TestAlg::handlePayload(std::string payload)
{
    Utils::log("got here", std::to_string(__LINE__));
    Utils::log(payload);
}

void TestAlg::proceedRound(int round)
{
    Utils::log("========= NEXT ROUND =========", round);
    rNode.flood(std::to_string(rNode.getUid())+" at round " + std::to_string(round));
}
