#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>
#include <string>
#include <poll.h>
#include <mutex>

class Utils
{

public:

    template<typename T>
    static void printVector(std::vector<T> vec);

    static std::string getAddressFromHost(std::string host);

    static int pollForFd(int fd, int time, int flag = POLLIN);

    template<typename Arg,typename... Args>
    static void log(Arg arg,Args... args)
    {
        logMutex.lock();

        std::cout << arg;
        using expander = int[];        
        (void)expander{0, (void(std::cout << ' ' << std::forward<Args>(args)), 0)...};
        std::cout << std::endl;

        logMutex.unlock();
    }

private:
    static std::mutex logMutex;
};

#endif
