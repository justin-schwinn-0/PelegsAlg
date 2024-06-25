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
/*

    void outGoingConnect();

    void sendMsg(std::string msg);

    void print();

    void acceptCon();

    void msgRx();

    void resetRemoteConnection();

*/
    void setRxFd(int fd)
    { mRxFd = fd;}

    void setTxFd(int fd)
    { mTxFd = fd;}

    void setInCon(bool c)
    { in_connected = c; }

    void setOutCon(bool c)
    { out_connected = c; }

    bool hasOutConnection();

    bool hasInConnection();
private:
    // hostname/ip of connection
    std::string hostname;
    uint32_t port;

    bool in_connected;
    bool out_connected;

    int mRxFd;
    int mTxFd;

};

#endif
