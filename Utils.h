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

    static std::vector<std::string> split(std::string str, std::string delim);
    
    static int strToInt(std::string s);
    
    template<typename T>
    static void printVector(std::vector<T> vec);

    static std::string getAddressFromHost(std::string host);

    static int pollForFd(int fd, int time, int flag = POLLIN);

    static void error(std::string s);

    template<typename Arg,typename... Args>
    static void log(Arg arg,Args... args)
    {
        static std::mutex logMutex;
        logMutex.lock();

        std::cout << arg;
        using expander = int[];        
        (void)expander{0, (void(std::cout << ' ' << std::forward<Args>(args)), 0)...};
        std::cout << std::endl;

        logMutex.unlock();
    }

};

#endif
