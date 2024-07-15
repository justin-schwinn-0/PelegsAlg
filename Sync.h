
#ifndef SYNC_H 
#define SYNC_H

#include "Node.h"
#include "Utils.h"
#include "SynchAlg.h"

#include <string>
#include <map>


struct SyncMsg
{
    int uid;
    std::string payload;
};

class Sync
{

public:
    Sync(int neighbors); 

    void msgHandler(std::string s);

    //parses the message, updating mHasRecvd and adding to payload cache if needed
    SyncMsg parseMsg(std::string);

    void progressRound();

    void init();

    template<class T>
    void setHandlers(T& t)
    {
        mHandlePayload = std::bind(&T::handlePayload,&t,std::placeholders::_1);
        mProceedRound = std::bind(&T::proceedRound,&t,std::placeholders::_1);
    }
private:
    //uid -> has recvd round msg
    std::map<int,bool>mHasRecvd;

    std::map<int,std::string> payloadCache;

    int mRound;

    std::function<void(SyncMsg)> mHandlePayload;
    std::function<void(int)> mProceedRound;

    int mKnownNeighbors;
};

#endif
