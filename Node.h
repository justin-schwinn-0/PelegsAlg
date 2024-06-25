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

    void listen();

    void connectNeighbors();

    Connection getOwnConnection();

    void addConnection(Connection c);

    int getUid();

    void print();

    void handleMsg();

    void flood(std::string str);
private:
    int mUid;
    Connection mListener;
    std::vector<Connection> mNeighbors;
};

#endif
