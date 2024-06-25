#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>

#include "Connection.h"

class Node
{
public:
    Node(int uid, Connection con):
        mUid(uid),
        mListener(con)
    {
    }

    void openSocket();

    void connectNeighbors();
    void acceptNeighbors();



    bool outConnectedToNeighbors();
    bool inConnectedToNeighbors();
    Connection getOwnConnection();
    int getUid();

    void flood(std::string str);

    void print();
    void addConnection(Connection c);
private:
    int mUid;
    Connection mListener;
    std::vector<Connection> mNeighbors;

    int mListenFd;
    // this probably isn't right
};

#endif
