
#ifndef SYNC_H 
#define SYNC_H


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
