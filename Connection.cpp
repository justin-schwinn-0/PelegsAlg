#include "Connection.h"
#include "Utils.h"

#include <iostream>
#include <thread>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>

Connection::Connection(std::string h, int p) : 
    hostname(h),
    port(p)
{
}

Connection::~Connection()
{
}

void Connection::sendMsg(std::string msg)
{
    
    std::string addr = Utils::getAddressFromHost(hostname);

    struct sockaddr_in serverAddress;
    memset(&serverAddress,0,sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = port;
    serverAddress.sin_addr.s_addr = inet_addr(addr.c_str());

    int sd= socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if(sd < 0)
    {
        Utils::log( "couldn't make SCTP socket!" ); 
        return;
    }

    int ret;
    do
    {
        sleep(2);
        ret = connect(sd,(struct sockaddr*)&serverAddress,sizeof(serverAddress));
    }
    while(ret == ECONNREFUSED);

    if(ret < 0)
    {
        Utils::error("connect failed");
    }

    ret = sctp_sendmsg(sd,(void *)msg.c_str(),strlen(msg.c_str())+1,NULL,0,0,0,0,0,0);

    if(ret < 0)
    {
        Utils::error("send failed");
    }

    close(sd);
}


void Connection::print()
{
    std::cout << "{ " << hostname << " " << port << " }";
}
