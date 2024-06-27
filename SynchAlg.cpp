#include "SynchAlg.h"


std::string SynchAlg::wrapPayload(std::string payload,int round)
{
    return std::to_string(rNode.getUid()) + "::" + std::to_string(round) + "::" + payload;
}
