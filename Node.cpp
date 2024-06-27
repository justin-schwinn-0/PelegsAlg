#include "Node.h"
#include "Utils.h"

#include <iostream>
#include <thread>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <poll.h>
#include <sys/select.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>


void Node::openSocket()
{
    struct sockaddr_in serverSocket = {AF_INET,INADDR_ANY};
    serverSocket.sin_port = mListener.getPort();

    struct sctp_initmsg init;
    init.sinit_num_ostreams = 5;
    init.sinit_max_instreams = 5;
    init.sinit_max_attempts = 4;

    mListenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

    if(mListenFd < 0)
    {
        Utils::log( "couldn't make SCTP socket!" ); 
        return;
    }

    int ret = bind(mListenFd, (struct sockaddr*)&serverSocket,sizeof(serverSocket));
    if(ret < 0)
    {
        Utils::log( "coudn't bind socket: " , strerror(errno) );
        return;
    }

    ret = setsockopt(mListenFd, IPPROTO_SCTP, SCTP_INITMSG, &init,sizeof(init));
    if(ret < 0)
    {
        Utils::log( "coudn't set socket: " , strerror(errno) );
        return;
    }


    ret = listen(mListenFd, init.sinit_max_instreams);
    if(ret < 0)
    {
        Utils::log( "coudn't listen!: " , strerror(errno) );
        return;
    }

    Utils::log( "socket open!" );
}

void Node::acceptNeighbors()
{
    if(mListenFd < 0)
    {
        Utils::log( "bad listener!" , mListenFd );
        return;
    }


    while(true)
    {
        int rxFd = accept(mListenFd, (struct sockaddr*)NULL,NULL);

        if(rxFd < 0)
        {
            Utils::log( "couldn't accept connection: " , strerror(errno) );
            return;
        }

        const int bufSize = 128;
        char buf[bufSize];

        struct sctp_sndrcvinfo sndrcvinfo;
        int flags;
        
        int in = sctp_recvmsg(rxFd,buf,bufSize,NULL,0,&sndrcvinfo,&flags);

        if(in > 0)
        {
            std::string strMsg(buf);
            msgHandler(strMsg);
        }

        close(rxFd);
    }
}

Connection Node::getOwnConnection()
{
    return mListener;
}

int Node::getUid()
{
    return mUid;
}

void Node::print()
{
    std::cout << mUid << " ";
    mListener.print();
    std::cout << "\n\t";

    for(auto& c : mNeighbors)
    {
        c.print();
    }

    std::cout << std::endl;
}

void Node::connectAll()
{
    for(auto& n : mNeighbors)
    {
        n.makeConnection();
    }
}

void Node::addConnection(Connection c)
{
    mNeighbors.push_back(c);
}

void Node::flood(std::string str)
{
    for(auto& n : mNeighbors)
    {
        n.sendMsg(str);
    }
}
