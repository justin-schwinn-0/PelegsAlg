#ifndef TEST_ALG_H
#define TEST_ALG_H

#include <string>

#include "SynchAlg.h"

class TestAlg : SynchAlg
{
public:
    TestAlg(Node& n);

    void handlePayload(std::string payload);

    void proceedRound();

private:
    int roundNum;
};

#endif
