
#ifndef SYNC_H 
#define SYNC_H

#include "Node.h"
#include "Utils.h"

#include <string>
#include <map>


class Sync
{

public:
    Sync(int neighbors,Node& n); 

    void msgHandler(std::string s);

    //parses the message, updating mHasRecvd and adding to payload cache if needed
    std::string parseMsg(std::string);

    void progressRound();

    std::string wrapPayload(std::string payload);

    void init();

    void affixVector(int uid);

    template<class T>
    void setHandlers();
    {
        mHandlePayload = &T::handlePayload;
        mProceedRound = &T::proceedRound;
    }
private:
    //uid -> has recvd round msg
    std::map<int,bool>mHasRecvd;

    std::map<int,std::string> payloadCache;

    int mRound;
    Node& rNode;

    bool affixedVectors;

    std::function<void(std::string)> mHandlePayload;
    std::function<void()> mProceedRound;
};

#endif
