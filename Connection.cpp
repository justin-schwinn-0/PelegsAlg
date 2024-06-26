#include "Connection.h"
#include "Utils.h"

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


    std::string addr = Utils::getAddressFromHost(hostname);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = port;
    serverAddress.sin_addr.s_addr = inet_addr(addr.c_str());

    mTxFd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(mTxFd < 0)
    {
        std::cout << "couldn't make SCTP socket!" << std::endl; 
        close(mTxFd);
        mTxFd = -1;
        return;
    }

    int ret = connect(mTxFd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    if(ret < 0)
    {
        std::cout << "coudn't connect to socket: " << strerror(errno) << std::endl;
        close(mTxFd);
        mTxFd = -1;
        return;
    }

    std::cout << "outgoing connection with  " << addr << std::endl;
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

void Connection::msgRx()
{
    while(true)
    {
        std::cout << "waiting for message from " << hostname << " ..." << std::endl;
        struct sctp_sndrcvinfo sndrcv;
        char buf[1024];
        int flags;
        int in = sctp_recvmsg(mTxFd,buf,sizeof(buf),NULL,0,&sndrcv,&flags);
        if(in != -1)
        {
            std::cout << "rx msg: " << buf << std::endl;
            std::cout << "from fd: " << mTxFd << std::endl;
        }
        else
        {
            std::cout << "message error: " << strerror(errno) << std::endl;
        }
    }
}

void Connection::print()
{
    std::cout << "{ " << hostname << " " << port << " }";
}

bool Connection::hasInConnection()
{
    if(mRxFd >= 0)
    {
        return true;
    }
    return false;
}

bool Connection::hasOutConnection()
{
    if(mTxFd >= 0)
    {
        return true;
    }
    return false;
}
