
#ifndef SYNC_H 
#define SYNC_H

#include "Node.h"
#include "Utils.h"

#include<string>


class Sync
{

public:
    Sync(const Node& n) : rNode(n)
    {
    }

    void msgHandler(std::string s);

private:
    int round;
    const Node& rNode;
};

#endif
