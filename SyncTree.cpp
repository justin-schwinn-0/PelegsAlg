#include "SyncTree.h"
#include "Node.h"
#include "Utils.h"

SyncTree::SyncTree(int neighbors) : 
    mKnownNeighbors(neighbors)
{
}

void SyncTree::msgHandler(std::string s)
{

    auto data =  Utils::split(s,"--");

    if(data[0] == "par") // if recived parent
    {
        int pid = Utils::strToInt(data[1]);
        int lid = Utils::strToInt(data[2]);
        parentUid = pid;
        leaderUid = lid;

        Utils::log("found parent and leader",parentUid,leaderUid);
    }
    else if( data[0] == "child") // ack parent
    {
        
    }
    else if(data[0] == "ref") // refuse parent
    {

    }
}

std::string SyncTree::parseMsg(std::string r)
{
