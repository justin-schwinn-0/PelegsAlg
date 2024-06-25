#include "Node.h"

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


void Node::openSocket()
{
    struct sockaddr_in serverSocket = {AF_INET,INADDR_ANY};
    serverSocket.sin_port = mListener.getPort();

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
        std::cout << "coudn't bind socket: " << strerror(errno) << std::endl;
        return;
    }

    ret = setsockopt(mListenFd, IPPROTO_SCTP, SCTP_INITMSG, &init,sizeof(init));
    if(ret < 0)
    {
        std::cout << "coudn't set socket: " << strerror(errno) << std::endl;
        return;
    }


    ret = listen(mListenFd, init.sinit_max_instreams);
    if(ret < 0)
    {
        std::cout << "coudn't listen!: " << strerror(errno) << std::endl;
        return;
    }

    std::cout << "socket open!" << std::endl;
}

void Node::connectNeighbors()
{
    for(auto& n : mNeighbors)
    {
        n.outGoingConnect();
    }
}

void Node::acceptNeighbors()
{
    if(mListenFd < 0)
    {
        std::cout << "bad listener!" << mListenFd << std::endl;
        return;
    }

    std::cout << "\n\nwaiting for connection..." << std:: endl;
    struct sockaddr_in socketAddress;
    int addrLength;
    int rxFd = accept(mListenFd, (struct sockaddr*)&socketAddress,(socklen_t*)&addrLength);

    std::cout << "accept connection on port\t\t" << socketAddress.in_addr.s_addr << std::endl;

    if(rxFd < 0)
    {
        std::cout << "coudn't accept connection: " << strerror(errno) << std::endl;
        return;
    }

    //id which connetion was accepted and set stuff right from there
}

void Node::msgRx()
{
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
        std::cout << "message error: " << strerror(errno) << std::endl;
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

    std::cout<< std::endl;
}

void Node::addConnection(Connection c)
{
    mNeighbors.push_back(c);
}

void Node::flood(std::string str)
{
    for(auto& ne : mNeighbors)
    {
        ne.msgTx(str);
    }
}

bool Node::outConnectedToNeighbors()
{
    bool con = true;
    for(auto& n : mNeighbors)
    {
        if(!n.hasOutConnection())
        {
            con = false;
        }
    }

    return con;
}

bool Node::inConnectedToNeighbors()
{
    bool accept = true;
    for(auto& n : mNeighbors)
    {
        if(!n.hasInConnection())
        {
            accept = false;
        }
    }

    return accept;
}
