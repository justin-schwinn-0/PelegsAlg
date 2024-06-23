#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>

class Connection
{    
public:
    Connection(std::string h, int p) : 
        hostname(h),
        port(p)
    {

    }

    void Listen()
    {
        int listenFd;

        struct sockaddr_in serverSocket = {AF_INET,INADDR_ANY};
        serverSocket.sin_port = port;

        struct sctp_initmsg init = {5,5,4};

        listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);

        if(listenFd < 0)
        {
            std::cout << "couldn't make SCTP socket!" << std::endl; 
        }

        int ret = bind(listenFd, (struct sockaddr*)&serverSocket,sizeof(serverSocket));
        if(ret < 0)
        {
            std::cout << "coudn't bind socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        }

        ret = setsockopt(listenFd, IPPROTO_SCTP, SCTP_INITMSG, &init,sizeof(init));
        if(ret < 0)
        {
            std::cout << "coudn't set socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        }


        ret = listen(listenFd, init.sinit_max_instreams);
        if(ret < 0)
        {
            std::cout << "coudn't listen!: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        }
    }

    void Connect()
    {
        int connFd;

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = port;
        serverAddress.sin_addr.s_addr = inet_addr(hostname.c_str());


        connFd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
        if(connFd < 0)
        {
            std::cout << "couldn't make SCTP socket!" << std::endl; 
        }

        int ret = connect(connFd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
        if(ret < 0)
        {
            std::cout << "coudn't connect to socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        }

    }

private:
    // hostname/ip of connection
    std::string hostname;

    uint32_t port;

};

#endif
