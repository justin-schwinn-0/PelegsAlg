#ifndef SYNCH_ALG_H
#define SYNCH_ALG_H

#include <string>

class SynchAlg
{
public:
    std::string wrapPayload(std::string payload);
    virtual void handlePayload(std::string str) = 0;

    virtual void proceedRound() = 0;

private:
    
    Node& rNode;
};

#endif
