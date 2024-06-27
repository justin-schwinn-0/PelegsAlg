#ifndef SYNCH_ALG_H
#define SYNCH_ALG_H

#include <string>

#include "Node.h"
class SynchAlg
{
public:
    std::string wrapPayload(std::string payload,int round);

    virtual void handlePayload(std::string str) = 0;

    virtual void proceedRound(int round) = 0;

private:
    Node& rNode;
};

#endif
