
#ifndef SYNC_H 
#define SYNC_H

#include "Node.h"
#include "Utils.h"

#include<string>


class Sync
{

public:
    Sync(int neighbors,const Node& n); 

    void msgHandler(std::string s);

private:
    //uid -> has recvd round msg
    std::vector<std::pair<int,bool>> mHasRecvd;

    int round;
    const Node& rNode;
};

#endif
