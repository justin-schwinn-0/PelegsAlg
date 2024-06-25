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


void Node::listen()
{
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
}

void Node::acceptNeighbors()
{
    while(!hasAcceptedNeighbors())
    {
        if(mListenFd < 0)
        {
            std::cout << "bad listener!" << mListenFd << " " << hostname << std::endl;
            return;
        }

        if(hasAccepted())
        {
            return;
        }

        std::cout << "\n\nwaiting for connection..." << std:: endl;

        int rxFd = accept(mListenFd,(struct sockaddr*)NULL,NULL);

        if(rxFd < 0)
        {
            std::cout << "coudn't accept connection: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
            return;
        }
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
