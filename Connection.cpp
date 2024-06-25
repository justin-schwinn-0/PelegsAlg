
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

void Connection::openSocket()
{
    int mListenFd;

    struct sockaddr_in serverSocket = {AF_INET,INADDR_ANY};
    serverSocket.sin_port = port;

    struct sctp_initmsg init = {5,5,4};

    mListenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if(mListenFd < 0)
    {
        std::cout << "couldn't make SCTP socket!" << std::endl; 
        return;
    }

    int ret = bind(mListenFd, (struct sockaddr*)&serverSocket,sizeof(serverSocket));
    if(ret < 0)
    {
        std::cout << "coudn't bind socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        return;
    }

    ret = setsockopt(mListenFd, IPPROTO_SCTP, SCTP_INITMSG, &init,sizeof(init));
    if(ret < 0)
    {
        std::cout << "coudn't set socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        return;
    }


    ret = listen(mListenFd, init.sinit_max_instreams);
    if(ret < 0)
    {
        std::cout << "coudn't listen!: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        return;
    }
    
}

void Connection::acceptMsg()
{
    if(mListenFd < 0)
    {
        std::cout << "bad listener!" << mListenFd << " " << hostname << std::endl;
        return;
    }
    struct sctp_sndrcvinfo sndrcv;
    char buf[1024];

    std::cout << "waiting for connection..." << std:: endl;

    int connFd = accept(mListenFd,(struct sockaddr*)NULL,NULL);

    if(connFd < 0)
    {
        std::cout << "coudn't accept connection: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        return;
    }
    int flags;
    int in = sctp_recvmsg(connFd,buf,sizeof(buf),NULL,0,&sndrcv,&flags);
    if(in > 0)
    {
        std::cout << "rx msg: " << buf << std::endl;
    }
}

bool Connection::isConnected()
{
    if(mConFd == -1)
    {
        return false;
    }
    return true;
}

void Connection::Connect()
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

    mConFd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(mConFd < 0)
    {
        std::cout << "couldn't make SCTP socket!" << std::endl; 
        mConFd = -1;
        return;
    }

    int ret = connect(mConFd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    if(ret < 0)
    {
        std::cout << "coudn't connect to socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "connected with fd " << mConFd << std::endl;
}

void Connection::sendMsg(std::string msg)
{
    if(mConFd < 0)
    {
        std::cout << "No connected to host!" << std::endl;
    }
    else
    {
        int ret = sctp_sendmsg(mConFd,(void *)msg.c_str(), strlen(msg.c_str())+1,NULL,0,0,0,0,0,0);
        if( ret < 0)
        {
            std::cout << "couldn't send message: " << strerror(errno) << std::endl;
        }
    }
}

void Connection::print()
{
    std::cout << "{ " << hostname << " " << port << " }";
}
