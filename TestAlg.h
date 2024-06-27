#ifndef TEST_ALG_H
#define TEST_ALG_H

#include "SynchAlg.h"

class TestAlg : SynchAlg
{
    TestAlg(Node& n);

    void handlePayload(std::string payload);

    void proceedRound();

private:
    Node& rNode;

    int roundNum;
};

#endif
