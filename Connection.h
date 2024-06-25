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

    void sendMsg(std::string msg);


    void acceptCon();


    void resetRemoteConnection();

*/

    void outGoingConnect();

    void setTxFd(int fd)
    { mTxFd = fd;}

    void setRxFd(int fd)
    { mRxFd = fd;}

    void msgTx(std::string msg);

    bool hasOutConnection();

    bool hasInConnection();

    uint32_t getPort()
    { return port; }

    const std::string& getHostname()
    { return hostname; }

    void print();
private:
    // hostname/ip of connection
    std::string hostname;
    uint32_t port;

    int mTxFd = -1;
    int mRxFd;

};

#endif
