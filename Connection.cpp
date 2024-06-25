
#include "Connection.h"

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>

void Connection::outGoingConnect()
{

    if(hasOutConnection())
    {
        return;
    }

    struct addrinfo *result,hints;

    int err = getaddrinfo(hostname.c_str(),NULL,NULL,&result); 

    if(err != 0)
    {
        std::cout << "getaddrinfo failed" << err << gai_strerror(errno) << std::endl;

        return;
    }

    char addr[50];

    void* p = &((struct sockaddr_in*) result->ai_addr)->sin_addr;

    inet_ntop(result->ai_family, p, addr, 50);

    std::cout << "try to connect to " << addr << std::endl;

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = port;
    serverAddress.sin_addr.s_addr = inet_addr(addr);

    mTxFd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(mTxFd < 0)
    {
        std::cout << "couldn't make SCTP socket!" << std::endl; 
        return;
    }

    int ret = connect(mTxFd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    if(ret < 0)
    {
        std::cout << "coudn't connect to socket: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "connected with fd " << mTxFd << std::endl;
}

void Connection::msgTx(std::string msg)
{
    if(mTxFd < 0)
    {
        std::cout << "No connected to host!" << std::endl;
    }
    else
    {
        int ret = sctp_sendmsg(mTxFd,(void *)msg.c_str(), strlen(msg.c_str())+1,NULL,0,0,0,0,1000,0);
        if( ret < 0)
        {
            std::cout << "couldn't send message: " << strerror(errno) << std::endl;
        }
        else
        {
            std::cout << "sent " << msg << " to " << hostname << std::endl;
        }
    }
}

void Connection::print()
{
    std::cout << "{ " << hostname << " " << port << " }";
}

bool Connection::hasInConnection()
{
    return in_connected;
}

bool Connection::hasOutConnection()
{
    return out_connected;
}
