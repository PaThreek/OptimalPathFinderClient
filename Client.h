#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <vector>

namespace Client
{

typedef std::pair<int, int> Point;
typedef std::vector<Point> Path;

class Client
{
public:
    Client();
    virtual ~Client();

    void start();
    void requestOptimalPath(int32_t x1, int32_t y1, int32_t x2, int32_t y2);
    void visualizePathToPng(const Path& path);

private:
    int createSocket();
    void writeData(int32_t* buffer);
    int readData(int32_t* buffer);

    int mServerFD;
    struct sockaddr_in mAddress;
};

}

#endif