#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <poll.h>

class Utils
{

public:

template<typename T>
static void printVector(std::vector<T> vec);

static std::string getAddressFromHost(std::string host);

static int pollForFd(int fd, int time, int flag = POLLIN);

};

#endif
