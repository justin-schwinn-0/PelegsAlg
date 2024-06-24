#ifndef CONNECTION_H
#define CONNECTION_H

#include <iostream>
#include <unistd.h>
#include <string>
#include <cerrno>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
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
        
        bool msgRx = false;
        while(!msgRx)
        {
            struct sctp_sndrcvinfo sndrcv;
            char buf[1024];

            std::cout << "waiting for connection..." << std:: endl;

            int connFd = accept(listenFd,(struct sockaddr*)NULL,NULL);

            if(connFd < 0)
            {
                std::cout << "coudn't accept connection: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
            }
            int flags;
            int in = sctp_recvmsg(connFd,buf,sizeof(buf),NULL,0,&sndrcv,&flags);
            if(in > 0)
            {
                std::cout << "rx msg: " << buf << std::endl;
            }

            close(connFd);


        }
    }

    void Connect()
    {
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = port;
        serverAddress.sin_addr.s_addr = inet_addr(hostname.c_str());


        mConFd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
        if(mConFd < 0)
        {
            std::cout << "couldn't make SCTP socket!" << std::endl; 
        }

        int ret = connect(mConFd, (struct sockaddr*)&serverAddress,sizeof(serverAddress));
        if(ret < 0)
        {
            std::cout << "coudn't connect to socket: " << hostname << " " << port  << " error: " << strerror(errno) << std::endl;
        }
    }

    void sendMsg(std::string msg)
    {
        if(mConFd < 0)
        {
            std::cout << "No connected to host!" << std::endl;
        }
        else
        {
            int ret = sctp_sendmsg(mConFd,(void *)msg.c_str(), strlen(msg.c_str())+1,NULL,0,0,0,0,0,0);
            if( ret < 0)
            {
                std::cout << "couldn't send message: " << strerror(errno) << std::endl;
            }
        }
    }



private:
    // hostname/ip of connection
    std::string hostname;

    int mConFd;

    uint32_t port;

};

#endif
