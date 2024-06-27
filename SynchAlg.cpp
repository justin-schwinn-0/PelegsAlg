#include "SynchAlg.h"


std::string SynchAlg::wrapPayload(std::string payload)
{
    return std::to_string(rNode.getUid()) + "::" + std::to_string(mRound) + "::" + payload;
}
