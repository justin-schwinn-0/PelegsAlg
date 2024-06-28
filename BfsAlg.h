#ifndef BFS_ALG_H
#define BFS_ALG_H

#include <string>

#include "Node.h"
class BfsAlg
{
public:
    BfsAlg(Node& n); 

    virtual void handlePayload(std::string str) = 0;

    virtual void proceedRound(int round) = 0;

private:
    int parentUid;
};

#endif
