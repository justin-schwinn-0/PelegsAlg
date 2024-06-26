#include "Connection.h"
#include "Utils.h"

#include <iostream>
#include <mutex>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>

Connection::Connection(std::string h, int p) : 
    hostname(h),
    port(p)
{
}

Connection::~Connection()
{
    if(isConnected())
    {
        close(mConFd);
    }
}
void Connection::outGoingConnect()
{

    if(isConnected())
    {
        return;
    }

    
    std::string addr = Utils::getAddressFromHost(hostname);

    struct sockaddr_in serverAddress;
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = port;
    serverAddress.sin_addr.s_addr = inet_addr(addr.c_str());

    int sd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(sd < 0)
    {
        std::cout << "couldn't make SCTP socket!" << std::endl; 
        close(sd);
        sd = -1;
        return;
    }

    int ret = connect(sd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    if(ret < 0)
    {
        std::cout << "coudn't connect to socket: " << strerror(errno) << std::endl;
        close(sd);
        sd = -1;
        return;
    }

    setConnection(sd,serverAddress);

    

    //std::cout << "outgoing connection with  " << addr << std::endl;
}

void Connection::msgTx(std::string msg)
{
    std::cout << "try send" << std::endl;
    if(mConFd < 0)
    {
        std::cout << "No connected to host!" << std::endl;
    }
    else
    {
        int ret = sctp_sendmsg(mConFd,(void *)msg.c_str(), strlen(msg.c_str())+1,NULL,0,0,0,0,1000,0);
        if( ret < 0)
        {
            std::cout << "couldn't send message: " << strerror(errno) << std::endl;
        }

        else
        {
            std::cout << "sent {" << msg << "} to " << hostname << std::endl;
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
        int in = sctp_recvmsg(mConFd,buf,sizeof(buf),NULL,0,&sndrcv,&flags);
        if(in != -1)
        {
            std::cout << "rx msg: " << buf << std::endl;
            std::cout << "from fd: " << mConFd << std::endl;
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

bool Connection::isConnected()
{
    if(mConFd >= 0)
    {
        return true;
    }
    return false;
}

void Connection::setConnection(int fd,sockaddr* farEnd)
{
    if(!isConnected())
    {
        mConFd = fd;
        mFarAddress= farEnd;
    }
}
