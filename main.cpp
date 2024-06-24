#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <sstream>

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

void readConfig(std::string configFile)
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
    printVector(lines);

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

    std::cout << "num Nodes: " << numNodes << std::endl;
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
            std::cout << "adding node " << uid << std::endl;

        }
        else
        {
            std::cout << "node Line " << i << " is invalid" << std::endl;
        }
        
    }

    //nodes[0].listen();

    Connection c = {"10.176.69.36",2234};

    c.Connect();
    c.sendMsg("test!");


}

int main()
{
    readConfig("testConfig.txt");
    return 0;
}
