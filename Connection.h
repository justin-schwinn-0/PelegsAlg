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

    void Listen();

    void Connect();

    void sendMsg(std::string msg);

    void print();



private:
    // hostname/ip of connection
    std::string hostname;

    int mConFd;

    uint32_t port;

};

#endif
