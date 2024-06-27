#include "SynchAlg.h"

SynchAlg::SynchAlg(Node& n) : 
    rNode(n)
{

}

std::string SynchAlg::wrapPayload(std::string payload,int round)
{
    
    Utils::log("wrapping payload {",payload,"} ",round);
    return std::to_string(rNode.getUid()) + "::" + std::to_string(round) + "::" + payload;
}
