
#ifndef SYNC_H 
#define SYNC_H

#include "Node.h"
#include "Utils.h"

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

    void affixVector(int uid);

    template<class T>
    void setHandlers(T& t)
    {
        mHandlePayload = std::bind(&T::handlePayload,t,std::placeholders::_1);
        mProceedRound = std::bind(&T::proceedRound,t,std::placeholders::_1);
    }
private:
    //uid -> has recvd round msg
    std::map<int,bool>mHasRecvd;

    std::map<int,std::string> payloadCache;

    int mRound;

    bool affixedVectors;

    std::function<void(std::string)> mHandlePayload;
    std::function<void(int)> mProceedRound;
};

#endif
