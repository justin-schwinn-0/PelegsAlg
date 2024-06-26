#include "Node.h"
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

    struct sockaddr_in socketAddress;
    int addrLength = sizeof(socketAddress);
    int rxFd = accept(mListenFd, (struct sockaddr*)&socketAddress,(socklen_t*)&addrLength);

    if(rxFd < 0)
    {
        std::cout << "coudn't accept connection: " << strerror(errno) << std::endl;
        return;
    }


    struct sockaddr addr;
    int peerLen = sizeof(addr);
    if(getpeername(rxFd,&addr,(socklen_t*)&peerLen) == 0)
    {
        char farAddress[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,&(socketAddress.sin_addr),farAddress,INET_ADDRSTRLEN);
        
        for(auto& con : mNeighbors)
        {
            if(!con.hasInConnection())
            {
                // if con.hostname and socketAddress resolve to the same ip...
                std::string conAddr = Utils::getAddressFromHost(con.getHostname());


                std::string farAddressStr(farAddress);
                if(conAddr == farAddressStr)
                {
                    con.setRxFd(rxFd);
                    std::cout << "connected with  " << farAddress << std::endl;
                }
            }
        }
    }
    else
    {
        std::cout << "could not get Peer name!" << std::endl;
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
