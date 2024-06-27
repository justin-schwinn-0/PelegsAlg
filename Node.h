#ifndef NODE_H
#define NODE_H

#include <vector>
#include <string>
#include <functional>

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

    void acceptNeighbors();

    Connection getOwnConnection();
    int getUid();

    void flood(std::string str);

    void print();
    void addConnection(Connection c);

    void setHandler(std::function<void(std::string)> fun)
    { msgHandler = fun;}

    int getNeighborsSize()
    { return mNeighbors.size(); }

    void connectAll();

    void listenToNeighbors();

    void recvMsg(int fd);

private:
    int mUid;
    Connection mListener;
    std::vector<Connection> mNeighbors;

    int mListenFd;
    // this probably isn't right
    
    std::function<void(std::string)> msgHandler;

    std::vector<int> openRcv;
};

#endif
