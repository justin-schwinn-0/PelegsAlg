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

    rNode.flood(std::to_string(n.getUid())+" at round " + std::to_string(roundNum));
}
