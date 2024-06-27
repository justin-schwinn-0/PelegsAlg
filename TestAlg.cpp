#include "TestAlg.h"

#include "Node.h"
#include "Utils.h"

TestAlg::TestAlg(Node& n) : 
    rNode(n),
    roundNum(0)
{
}

void TestAlg::handlePayload(std::string payload)
{
    Utils::log(payload);
}

void TestAlg::proceedRound()
{
    roundNum++;

    Utils::log("========= NEXT ROUND =========", roundNum);
    rNode.flood(std::to_string(rNode.getUid())+" at round " + std::to_string(roundNum));
}
