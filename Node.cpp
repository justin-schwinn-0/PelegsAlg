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

void Node::initMessageThreads()
{
    for(auto& con: mNeighbors)
    {
        std::thread msgRxer(&Connection::msgRx,std::ref(con));
        msgRxer.detach();
    }
    std::cout << "init msg threads!" << std::endl;
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


    if(retfd == 0)
    {
        std::cout << "no connecting neighbors, moving on..." << std::endl;
        return;
    }

    // blocking call to poll with timeout
    
    pollfd pfds[1];

    pfds[0].fd= mListenFd;
    pfds[0].events= POLLIN;

    int retfd = poll(pfds,1,5000); // 5s timeout
    
    if(retfd == 0)
    {
        std::cout << "no connection found, moving on..." << std::endl;
        return;
    }


    int rxFd = accept(mListenFd, (struct sockaddr*)&socketAddress,(socklen_t*)&addrLength);

    std::cout << "try connect on " << retfd << " " << mListenFd << std::endl;

    if(rxFd < 0)
    {
        std::cout << "couldn't accept connection: " << strerror(errno) << std::endl;
        return;
    }

    struct sockaddr addr;
    int peerLen = sizeof(addr);
    if(getpeername(rxFd,&addr,(socklen_t*)&peerLen) == 0)
    {
        bool connectionAccepted = false;
        char farAddress[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,&(socketAddress.sin_addr),farAddress,INET_ADDRSTRLEN);
        
        for(auto& con : mNeighbors)
        {
            if(!con.isConnected())
            {
                // if con.hostname and socketAddress resolve to the same ip...
                std::string conAddr = Utils::getAddressFromHost(con.getHostname());

                std::string farAddressStr(farAddress);
                if(conAddr == farAddressStr)
                {
                    con.setConnection(rxFd,addr);
                    connectionAccepted =true;
                    std::cout << "incomming connection with  " << farAddress << std::endl;
                }
            }
        }

        if(!connectionAccepted)
        {
            std::cout << "connection not added" << std::endl;
            close(rxFd);
        }
        return;
    }
    else
    {
        std::cout << "could not get Peer name!" << std::endl;
        close(rxFd);
        return;
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

bool Node::connectedToNeighbors()
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
