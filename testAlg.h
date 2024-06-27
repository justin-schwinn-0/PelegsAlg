#ifndef TEST_ALG_H
#define TEST_ALG_H

#include "SynchAlg.h"

class TestAlg : SynchAlg
{

    void handlePayload(std::string str);

    void proceedRound();
};

#endif
