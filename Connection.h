#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <mutex>

#include <sys/socket.h>
#include <sys/types.h>

class Connection
{    
public:
    Connection(std::string h, int p); 

    Connection(); 
    ~Connection();
/*

    void sendMsg(std::string msg);


    void acceptCon();


    void resetRemoteConnection();

*/

    void outGoingConnect();

    void setConnection(int fd,sockaddr farEnd);

    void msgTx(std::string msg);

    bool isConnected();

    uint32_t getPort()
    { return port; }

    const std::string& getHostname()
    { return hostname; }

    void print();
private:
    // hostname/ip of connection
    std::string hostname;
    uint32_t port;

    sockaddr mFarAddress;

    int mConFd = -1;
};

#endif
