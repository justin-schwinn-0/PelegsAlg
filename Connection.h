#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>

class Connection
{    
public:
    Connection(std::string h, int p) : 
        hostname(h),
        port(p)
    {

    }

    void openSocket();

    void Connect();

    void sendMsg(std::string msg);

    void print();

    void acceptCon();

    void resetRemoteConnection();

    bool isConnected();

    bool hasAccepted();

private:
    // hostname/ip of connection
    std::string hostname;
    uint32_t port;

    int mTxFd =-1;
    int mRxFd =-1;
    int mListenFd =-1;

};

#endif
