
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

    //parses the message, updating mHasRecvd and adding to payload cache if needed
    std::string parseMsg(std::string);

    void progressRound();

    std::string wrapPayload(std::string payload);
private:
    //uid -> has recvd round msg
    std::vector<std::pair<int,bool>> mHasRecvd;

    std::vector<std::pair<int,std::string>> payloadCache;

    int mRound;
    const Node& rNode;
};

#endif
