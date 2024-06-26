#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>
#include <mutex>

#include <sys/socket.h>

class Connection
{    
public:
    Connection(std::string h, int p); 

    ~Connection();
/*

    void sendMsg(std::string msg);


    void acceptCon();


    void resetRemoteConnection();

*/

    void outGoingConnect();

    void setConnectionFd(int fd);

    void msgTx(std::string msg);
    void msgRx();

    bool isConnect();

    uint32_t getPort()
    { return port; }

    const std::string& getHostname()
    { return hostname; }

    void print();
private:
    // hostname/ip of connection
    std::string hostname;
    uint32_t port;

    std::mutex connection_mutex;
    int mConFd = -1;
};

#endif
