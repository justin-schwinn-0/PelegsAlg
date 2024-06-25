#include "Node.h"

#include <iostream>

void Node::listen()
{
   mListener.openSocket(); 
   std::cout << "I'm listening! " << mUid << std::endl;
}

void Node::connectNeighbors()
{
    for(Connection& con : mNeighbors)
    {
        con.outGoingConnect();
    }
}

void Node::acceptNeighbors()
{
    for(Connection& con : mNeighbors)
    {
        con.acceptCon();
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

void Node::handleMsg()
{
    std::cout << "node gets to line " << __LINE__ << std::endl;
    mListener.msgRx();
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
