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

    void acceptMsg();

    bool isConnected();

private:
    // hostname/ip of connection
    std::string hostname;

    int mConFd;
    int mListenFd;

    uint32_t port;
};

#endif
