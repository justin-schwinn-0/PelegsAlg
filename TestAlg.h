#ifndef TEST_ALG_H
#define TEST_ALG_H

#include <string>

#include "SynchAlg.h"
#include "Node.h"
#include "Utils.h"

class TestAlg : SynchAlg
{
public:
    TestAlg(Node& n);

    void handlePayload(std::string payload);

    void proceedRound();

private:
    Node& rNode;

    int roundNum;
};

#endif
