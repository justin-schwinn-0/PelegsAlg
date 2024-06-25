
#include "Connection.h"

void Connection::acceptCon()
{
}

void Connection::msgRx()
{
    if(!hasAccepted())
    {
        std::cout << "incomming connection not connected!" << std::endl;
        return;
    }
    struct sctp_sndrcvinfo sndrcv;
    char buf[1024];
    int flags;
    int in = sctp_recvmsg(mRxFd,buf,sizeof(buf),NULL,0,&sndrcv,&flags);
    if(in != -1)
    {
        std::cout << "rx msg: " << buf << std::endl;
    }
    else
    {
        std::cout << "message error: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
    }
}

bool Connection::isConnected()
{
    return in_connected;
}

bool Connection::hasAccepted()
{
    return out_connected;
}

void Connection::outGoingConnect()
{

    if(isConnected())
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

    int txFd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(mTxFd < 0)
    {
        std::cout << "couldn't make SCTP socket!" << std::endl; 
        resetRemoteConnection();
        return;
    }

    int ret = connect(txFd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    if(ret < 0)
    {
        std::cout << "coudn't connect to socket: " << strerror(errno) << std::endl;
        resetRemoteConnection();
        return;
    }

    std::cout << "connected with fd " << txFd << std::endl;
}

void Connection::sendMsg(std::string msg)
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
