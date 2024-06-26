#include "Utils.h"

#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>

template<typename T>
void Utils::printVector(std::vector<T> vec)
{
    for(auto t : vec)
    {
        std::cout << t << "\n"; 
    }
    std::cout << std::endl;
}

std::string Utils::getAddressFromHost(std::string host)
{
    struct addrinfo *result;

    int err = getaddrinfo(host.c_str(),NULL,NULL,&result); 

    if(err != 0)
    {
        std::cout << "getaddrinfo failed" << err << gai_strerror(errno) << std::endl;

        return "";
    }

    const int addrLen = 1024;
    char addr[addrLen];

    void* p = &((struct sockaddr_in*) result->ai_addr)->sin_addr;

    inet_ntop(result->ai_family, p, addr, addrLen);
    return addr;
}

void Utils::pollForFd(int fd, int time)
{
    pollfd pfds[1];

    pfds[0].fd= fd;
    pfds[0].events= POLLIN;

    
    return poll(pfds,1,time) // 5s timeout
}
