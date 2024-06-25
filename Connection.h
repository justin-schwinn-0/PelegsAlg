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

    void resetRemoteConnection();

    bool isConnected();

private:
    // hostname/ip of connection
    std::string hostname;

    int mConFd=-1;
    int mListenFd=-1;

    uint32_t port;
};

#endif
