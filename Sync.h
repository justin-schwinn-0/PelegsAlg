
#ifndef SYNC_H 
#define SYNC_H

#include "Node.h"
#include "Utils.h"
#include "SynchAlg.h"

#include <string>
#include <map>


struct payloadData
{
    payload(std::string str,int s) : payload(str), uid(s)
    std::string payload;
    int uid;
};

class Sync
{

public:
    Sync(int neighbors); 

    void msgHandler(std::string s);

    //parses the message, updating mHasRecvd and adding to payload cache if needed
    payloadData parseMsg(std::string);

    void progressRound();

    void init();

    template<class T>
    void setHandlers(T& t)
    {
        mHandlePayload = std::bind(&T::handlePayload,&t,std::placeholders::_1,std::placeholders::_2);
        mProceedRound = std::bind(&T::proceedRound,&t,std::placeholders::_1);
    }
private:
    //uid -> has recvd round msg
    std::map<int,bool>mHasRecvd;

    std::map<int,std::string> payloadCache;

    int mRound;

    std::function<void(int,std::string)> mHandlePayload;
    std::function<void(int)> mProceedRound;

    int mKnownNeighbors;
};

#endif
