#ifndef TEST_ALG_H
#define TEST_ALG_H

#include <string>

#include "SynchAlg.h"

class TestAlg : public SynchAlg
{
public:
    TestAlg(Node& n);

    void handlePayload(std::string payload);

    void proceedRound(int round);

};

#endif
