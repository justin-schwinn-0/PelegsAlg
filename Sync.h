
#ifndef SYNC_H 
#define SYNC_H

#include "Node.h"
#include "Utils.h"
#include "SynchAlg.h"

#include <string>
#include <map>



class Sync
{

public:
    Sync(int neighbors); 

    void msgHandler(std::string s);

    //parses the message, updating mHasRecvd and adding to payload cache if needed
    std::string parseMsg(std::string);

    void progressRound();

    void init();

    template<class T>
    void setHandlers(SynchAlg& t)
    {
        mHandlePayload = std::bind(&T::handlePayload,static_cast<T>(t),std::placeholders::_1);
        mProceedRound = std::bind(&T::proceedRound,static_cast<T>(t),std::placeholders::_1);
    }
private:
    //uid -> has recvd round msg
    std::map<int,bool>mHasRecvd;

    std::map<int,std::string> payloadCache;

    int mRound;

    std::function<void(std::string)> mHandlePayload;
    std::function<void(int)> mProceedRound;

    int mKnownNeighbors;
};

#endif
