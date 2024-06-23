#ifndef NODE_H
#define NODE_H

#include "Connection.h"
class Node
{
public:
    Node(int uid, Connection con):
        mUid(uid),
        mListener(con)
    {
    }

    void listen()
    {
       mListener.Listen(); 
       std::cout << "I'm listening! " << mUid << std::endl;
    }

    void connectNeighbors()
    {
        for(auto con : mNeighbors)
        {
            con.Connect();
        }
    }

private:
    int mUid;
    Connection mListener;
    std::vector<Connection> mNeighbors;
};

#endif
