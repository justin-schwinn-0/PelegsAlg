#ifndef SYNCH_ALG_H
#define SYNCH_ALG_H

class SynchAlg
{
public:
    virtual void handlePayload(std::string str) = 0;

    virtual void proceedRound() = 0;
};

#endif
