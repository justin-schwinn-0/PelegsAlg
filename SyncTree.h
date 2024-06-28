
#ifndef SYNC_TREE_H 
#define SYNC_TREE_H

#include "Node.h"
#include "Utils.h"

#include <string>
#include <map>



class SyncTree
{

public:
    SyncTree(int neighbors); 

    void msgHandler(std::string s);

    //parses the message, updating mHasRecvd and adding to payload cache if needed
    std::string parseMsg(std::string);

    void progressRound();

    void init();

private:
    int mKnownNeighbors;

    int parentUid;
    it leaderUid;
};

#endif
