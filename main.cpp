#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <functional>

#include "Node.h"
#include "Connection.h"
#include "Sync.h"
#include "TestAlg.h"
#include "PelegsAlg.h"
#include "BfsAlg.h"
#include "Utils.h"


Node readConfig(std::string configFile, int popId = -1)
{
    std::ifstream file(configFile) ;
    Utils::log( "reading file: " , configFile );

    std::string wholeFile;
    std::string line;

    while(std::getline(file,line))
    {
        wholeFile += line + "\n";
    }
    auto lines = Utils::split(wholeFile,"\n");



    // sanitize lines to remove comments
    
    for(auto it = lines.rbegin(); it != lines.rend(); it++)
    {
        // erase the while line if it starts with #
        std::string& str = *it;
        if(str.at(0) == '#')
        {
            lines.erase((it+1).base());
        }
        else
        {
           size_t pos = 0;
           // if we find a #, erase it and everything after
           if((pos = str.find("#")) != std::string::npos)
           {
               str.erase(str.begin()+pos,str.end());
           }
        }
    }
    // use lines to create N nodes, and return 1 for this process
    auto firstLine = Utils::split(lines[0]," ");

    int numNodes;
    if(firstLine.size() != 1)
    {
        // if length of the first line isn't 1,
        // then something when wrong in the sanitization 
        // or the config is invalid
        Utils::log( "First valid line of Config is too long!" );
    }
    else
    {
        std::istringstream iss(firstLine[0]);

        iss >> numNodes;
    }


    std::vector<Node> nodes;
    for(int i = 1; i < numNodes+1;i++)
    {
        auto splitNode = Utils::split(lines[i]," ");

        if(splitNode.size() == 3)
        {
            std::istringstream uidSS(splitNode[0]); 
            std::istringstream portSS(splitNode[2]); 

            int uid,port;

            uidSS >> uid;
            portSS >> port;

            Node n(uid,{uid,splitNode[1],port});
            nodes.push_back(n);
            //Utils::log( "adding node " , uid );

        }
        else
        {
            Utils::log( "node Line " , i , " is invalid" );
        }
        
    }

    for(int i = 0; i < nodes.size(); i++)
    {
        auto connections = Utils::split(lines[i+numNodes+1]," ");

        for(auto c : connections)
        {
            std::istringstream uidSS(c); 

            int neighborId;
            uidSS >> neighborId;


            // search all nodes for the right Node Id, and add that connection 
            for(auto n : nodes)
            {
                if(neighborId == n.getUid())
                {
                    nodes[i].addConnection(n.getOwnConnection());
                    //Utils::log( nodes[i].getUid() , " connects to " , neighborId );
                }
            }
        }
    }

    if(popId > 0)
    {
        for(auto n : nodes)
        {
            if(n.getUid() == popId)
            {
                return n;
            }
        }

        Utils::log( "could not find id: " , popId ); 
    }

    return nodes[0];
}


template<class T>
void runAlg(Node& n)
{
    n.print();
    n.openSocket();


    Sync sync(n.getNeighborsSize());

    T t(n);
    sync.setHandlers<T>(t);

    n.setHandler(std::bind(&Sync::msgHandler,sync,std::placeholders::_1));

    n.connectAll();

    n.acceptNeighbors();
    sync.init();
    n.listenToNeighbors(500);
}

int main(int argc,char** argv)
{
    int uid;
    if(argc == 2)
    {
        uid = std::stoi(argv[1]);

        auto n = readConfig("testConfig.txt",uid);

        runAlg<PelegsAlg>(n);
        Utils::log("FOUND LEADER");
	
	/*
        int parentId = -1;
        std::vector<int> childIds;
        int responsesLeft = n.getNeighborsSize(); 

        auto bfsLambda = [&](std::string msg)
        {
            auto data = Utils::split(msg,"==");
            int uid = Utils::strToInt(data[0]);
            if(data[1] == "parent")
            {
                if(parentId ==-1)
                {
                    parentId = uid;
                    Utils::log("parent is ",uid);
                    n.sendExcept(uid,std::to_string(n.getUid())+"==parent");
                    responsesLeft--;
                }
                else 
                {
		    Utils::log("ref parent ", uid);
                    n.sendTo(uid,std::to_string(n.getUid())+"==ref");
		    Utils::log("sent to ", uid);
                }
            }
            else if(data[1] == "ref")
            {
                Utils::log("refuse parentage of ",uid);    
                responsesLeft--;
            }
            else if(data[1] == "child")
            {
                childIds.push_back(uid);
                responsesLeft--;
            }

            if(responsesLeft == 0)
            {
                if(parentId != -1)
                {
			Utils::log("conf parent");
                    n.sendTo(parentId,std::to_string(n.getUid())+"==child");
                }
                n.finishAlg();
            }

	Utils::log("|");
        };

        n.print();
        n.openSocket();

        n.setHandler(bfsLambda);

        n.connectAll();

        n.acceptNeighbors();
        if(n.getUid() == 89)
        {
            n.flood(std::to_string(n.getUid())+"==parent");
        }
        n.listenToNeighbors(1500);

        Utils::log("parent", parentId);

        Utils::log("children:");
        Utils::printVector(childIds);*/
    }
    else
    {
        readConfig("testConfig.txt");
    }
    return 0;
}
