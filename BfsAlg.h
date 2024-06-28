#ifndef BFS_ALG_H
#define BFS_ALG_H

#include <string>

#include "Node.h"
#include "SynchAlg.h"
class BfsAlg : SynchAlg
{
public:
    BfsAlg(Node& n); 

    virtual void handlePayload(std::string str) = 0;

    virtual void proceedRound(int round) = 0;

private:
    int parentUid;
    bool mClaimChildren;
};

#endif