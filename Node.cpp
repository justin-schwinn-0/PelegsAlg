#include "Node.h"

#include <iostream>

void Node::listen()
{
   mListener.Listen(); 
   std::cout << "I'm listening! " << mUid << std::endl;
}

void Node::connectNeighbors()
{
    for(Connection con : mNeighbors)
    {
        con.Connect();
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

    for(auto c : mNeighbors)
    {
        c.print();
    }

    std::cout<< std::endl;
}

void Node::addConnection(Connection c)
{
    mNeighbors.push_back(c);
}
