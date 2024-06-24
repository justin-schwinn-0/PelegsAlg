#include "Node.h"


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

