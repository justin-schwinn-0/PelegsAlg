#include "TestAlg.h"

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

    n.flood(std::to_string(n.getUid())+" at round " + std::to_string(roundNum));
}
