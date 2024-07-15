#ifndef SYNCH_ALG_H
#define SYNCH_ALG_H

#include <string>

#include "Sync.h"
#include "Node.h"

struct SyncMsg;

class SynchAlg
{
public:
    SynchAlg(Node& n); 

    virtual void handlePayload(SyncMsg payload) = 0;


    virtual void proceedRound(int round) = 0;

protected:

    std::string wrapPayload(std::string payload,int round);

    Node& rNode;
};

#endif
