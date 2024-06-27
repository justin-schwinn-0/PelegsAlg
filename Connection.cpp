#include "Connection.h"
#include "Utils.h"

#include <iostream>
#include <thread>
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
        Utils::log( "couldn't make SCTP socket!" ); 
        return;
    }

    int ret;    
    do
    {
        if(errno == ECONNREFUSED)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        }
        ret = connect(sd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    }
    while(ret < 0 && errno == ECONNREFUSED);

    if(ret < 0)
    {
        Utils::log( "coudn't connect to socket: " , strerror(errno) );
        close(sd);
        return;
    }

    struct sockaddr address;
    int peerLen = sizeof(address);
    if(getpeername(sd,&address,(socklen_t*)&peerLen) == 0)
    {
        setConnection(sd,address);
        Utils::log( "outgoing connection with  " , addr );
    }
    else
    {
        Utils::log( "could not get Peer name!" );
        return;
    }

}

void Connection::msgTx(std::string msg)
{
    if(mConFd < 0)
    {
        Utils::log( "No connected to host!" );
    }
    else
    {
        int ret = Utils::pollForFd(mConFd,3000,POLLOUT);
        if(ret == 0)
        {
            Utils::log( "tx timed out..." );
        }
        else if(ret > 0)
        {
            ret = sctp_sendmsg(mConFd,(void *)msg.c_str(), msg.size(),NULL,0,0,0,0,1000,0);
        }
        else
        {
            Utils::log( "tx polling error: " , strerror(errno) );
        }

        if( ret < 0)
        {
            Utils::log( "couldn't send message: " , strerror(errno) );
        }

        else
        {
            Utils::log( "sent {" , msg , "} to " , hostname );
        }
    }
}

void Connection::msgRx()
{
    Utils::log( "just run retard" );
    while(true)
    {

        Utils::log( "waiting for message from " , hostname , " ..." );
        struct sctp_sndrcvinfo sndrcv;
        char buf[128];
        int flags;

        int in = -1;
        int ret =Utils::pollForFd(mConFd,3000,POLLIN);
        if(ret == 0)
        {   
            Utils::log( "rx timed out..." );
            
        }
        else if( ret > 0)
        {
            in = sctp_recvmsg(mConFd,buf,sizeof(buf),NULL,0,&sndrcv,&flags);
            if(in != -1)
            {
                std::string msg = buf;
                Utils::log( "msg size: " , msg.size() , " " , in);
                Utils::log( "rx msg: " , msg );
            }
            else
            {
                Utils::log( "rx message error ", hostname , " : " , strerror(errno) );

                switch(errno)
                {
                    case EBADF:
                        Utils::log( "fd: " , mConFd );
                        break;
                    default:
                        break;
                }
            }
        }
        else
        {
            Utils::log( "rx polling error: " , strerror(errno) );
        }

    }
}

void Connection::print()
{
    Utils::logMutex.lock();
    std::endl << "{ " << hostname << " " << port << " }";
    Utils::logMutex.unlock();
}

bool Connection::isConnected()
{
    if(mConFd >= 0)
    {
        return true;
    }
    return false;
}

void Connection::setConnection(int fd,sockaddr farEnd)
{
    if(!isConnected())
    {
        mConFd = fd;
        mFarAddress= farEnd;
        Utils::log( "setting connection FD " , mConFd );
    }
}
