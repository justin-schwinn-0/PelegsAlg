
#include "Connection.h"

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
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
    }

    int ret = bind(mListenFd, (struct sockaddr*)&serverSocket,sizeof(serverSocket));
    if(ret < 0)
    {
        std::cout << "coudn't bind socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
    }

    ret = setsockopt(mListenFd, IPPROTO_SCTP, SCTP_INITMSG, &init,sizeof(init));
    if(ret < 0)
    {
        std::cout << "coudn't set socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
    }


    ret = listen(mListenFd, init.sinit_max_instreams);
    if(ret < 0)
    {
        std::cout << "coudn't listen!: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
    }
    std::cout << "listening on"<< mListenFd << std::endl; 
    
}

void Connection::acceptMsg()
{
    bool msgRx = false;
    while(!msgRx)
    {
        struct sctp_sndrcvinfo sndrcv;
        char buf[1024];

        std::cout << "waiting for connection..." << std:: endl;

        int connFd = accept(mListenFd,(struct sockaddr*)NULL,NULL);

        if(connFd < 0)
        {
            std::cout << "coudn't accept connection: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        }
        int flags;
        int in = sctp_recvmsg(connFd,buf,sizeof(buf),NULL,0,&sndrcv,&flags);
        if(in > 0)
        {
            std::cout << "rx msg: " << buf << std::endl;
        }

        close(connFd);
    }
}

void Connection::Connect()
{
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = port;
    serverAddress.sin_addr.s_addr = inet_addr(hostname.c_str());


    mConFd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(mConFd < 0)
    {
        std::cout << "couldn't make SCTP socket!" << std::endl; 
    }

    int ret = connect(mConFd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
    if(ret < 0)
    {
        std::cout << "coudn't connect to socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
    }
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
