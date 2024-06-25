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
    while(!isConnectedToNeighbors())
    {
    }
}

void Node::acceptNeighbors()
{
    while(!hasAcceptedNeighbors())
    {
        if(mListenFd < 0)
        {
            std::cout << "bad listener!" << mListenFd << std::endl;
            return;
        }

        std::cout << "\n\nwaiting for connection..." << std:: endl;

        int rxFd = accept(mListenFd,(struct sockaddr*)NULL,NULL);

        if(rxFd < 0)
        {
            std::cout << "coudn't accept connection: " << strerror(errno) << std::endl;
            return;
        }

        //id which connetion was accepted and set stuff right from there
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
        ne.sendMsg(str);
    }
}

bool Node::isConnectedToNeighbors()
{
    bool con = true;
    for(auto& n : mNeighbors)
    {
        if(!n.isConnected())
        {
            con = false;
        }
    }

    return con;
}

bool Node::hasAcceptedNeighbors()
{
    bool accept = true;
    for(auto& n : mNeighbors)
    {
        if(!n.hasAccepted())
        {
            accept = false;
        }
    }

    return accept;
}
