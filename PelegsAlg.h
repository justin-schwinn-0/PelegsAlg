#ifndef PELEGS_ALG_H
#define PELEGS_ALG_H

#include <string>

#include "SynchAlg.h"

class PelegsAlg : public SynchAlg
{
public:
    TestAlg(Node& n);

    void handlePayload(std::string payload);

    void proceedRound(int round);

};

#endif
