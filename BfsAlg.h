#ifndef BFS_ALG_H
#define BFS_ALG_H

#include <string>

#include "Node.h"
#include "SynchAlg.h"
class BfsAlg : SynchAlg
{
public:
    BfsAlg(Node& n); 

    void handlePayload(std::string str);

    void proceedRound(int round);

    void rootTree();

    std::string parentStr();

private:
    int parentUid;
    int mLeader;
    bool mClaimChildren;


};

#endif
