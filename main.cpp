#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

#include "Node.h"
#include "Connection.h"


std::vector<std::string> split(std::string str, std::string delim)
{
    std::vector<std::string> splits;
    uint32_t prevPos = 0;

    size_t pos = 0;
    while ((pos = str.find(delim)) != std::string::npos) 
    {
        std::string token = str.substr(0, pos);
        if(!token.empty())
        {
            splits.push_back(token);
        }
        str.erase(0, pos + delim.length());
    }
    //add remainder of split string
    if(!str.empty())
    {
        splits.push_back(str);
    }

    return splits;
}

template<typename T>
void printVector(std::vector<T> vec)
{
    for(auto t : vec)
    {
        std::cout << t << "\n"; 
    }
    std::cout << std::endl;
}

Node readConfig(std::string configFile, int popId = -1)
{
    std::ifstream file(configFile) ;
    std::cout << "reading file: " << configFile << std::endl;

    std::string wholeFile;
    std::string line;

    while(std::getline(file,line))
    {
        wholeFile += line + "\n";
    }
    auto lines = split(wholeFile,"\n");



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
    auto firstLine = split(lines[0]," ");

    int numNodes;
    if(firstLine.size() != 1)
    {
        // if length of the first line isn't 1,
        // then something when wrong in the sanitization 
        // or the config is invalid
        std::cout << "First valid line of Config is too long!" << std::endl;
    }
    else
    {
        std::istringstream iss(firstLine[0]);

        iss >> numNodes;

    }


    std::vector<Node> nodes;
    for(int i = 1; i < numNodes+1;i++)
    {
        auto splitNode = split(lines[i]," ");

        if(splitNode.size() == 3)
        {
            std::istringstream uidSS(splitNode[0]); 
            std::istringstream portSS(splitNode[2]); 

            int uid,port;

            uidSS >> uid;
            portSS >> port;

            Node n(uid,{splitNode[1],port});
            nodes.push_back(n);
            //std::cout << "adding node " << uid << std::endl;

        }
        else
        {
            std::cout << "node Line " << i << " is invalid" << std::endl;
        }
        
    }

    for(int i = 0; i < nodes.size(); i++)
    {
        auto connections = split(lines[i+numNodes+1]," ");

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
                    //std::cout << nodes[i].getUid() << " connects to " << neighborId << std::endl;
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

        std::cout << "could not find id: " << popId << std::endl; 
    }

    return nodes[0];
}

void outConnections(Node& n)
{

    while(!n.outConnectedToNeighbors())
    {
        n.connectNeighbors();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
    std::cout << "set up out going connections!" << std::endl;
}

void inConnections(Node& n)
{

    while(!n.inConnectedToNeighbors())
    {
        n.acceptNeighbors();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }
    std::cout << "accepted incomming connections!" << std::endl;
}

int main(int argc,char** argv)
{
    int uid;
    if(argc == 2)
    {
        uid = std::stoi(argv[1]);

        auto n = readConfig("testConfig.txt",uid);

        n.print();
        n.openSocket();
        

        if(uid == 5)
        {
            std::cout << "testing connections" << std::endl;
            std::thread outConnector(outConnections,std::ref(n));
            outConnector.join();
        }
        else
        {
            std::cout << "testing accepts" << std::endl;
            std::thread inConnector(inConnections,std::ref(n));
            inConnector.join();
        }

        n.flood("hello from " + std::to_string(n.getUid()));
        std::cout << "done" << std::endl;

    }
    else
    {
        readConfig("testConfig.txt");
    }
    return 0;
}
