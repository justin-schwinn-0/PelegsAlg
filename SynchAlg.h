#ifndef SYNCH_ALG_H
#define SYNCH_ALG_H

#include <string>

#include "Node.h"
class SynchAlg
{
public:
    std::string wrapPayload(std::string payload);
    virtual void handlePayload(std::string str) = 0;

    virtual void proceedRound() = 0;
    
    virtual void incrementRound();

private:
    int mRound; 
    Node& rNode;
};

#endif
