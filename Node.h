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



    bool isConnectedToNeighbors();
    bool hasAcceptedNeighbors();
    Connection getOwnConnection();
    int getUid();

    void msgRx();
    void flood(std::string str);

    void print();
    void addConnection(Connection c);
private:
    int mUid;
    Connection mListener;
    std::vector<Connection> mNeighbors;

    int mListenFd;
};

#endif
