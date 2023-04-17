#include "Client.h"

#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

#include "fpng.h"
#include "stb_image_write.h"

namespace Client
{

constexpr int PORT = 8080;
constexpr int DATA_MAX_SIZE = 1024;
constexpr int ERROR_GETTING_PATH_RETURN_CODE = -1;
constexpr int SOCKET_CONNECTION_CREATION_ERROR_RESULT = -1;
constexpr int SOCKET_CONNECTION_CREATION_OK_RESULT = 0;

Client::Client()
: mServerFD(-1)
{

}

Client::~Client()
{

}

void Client::start()
{   
    std::cout << "Starting Client" << std::endl;

    while(createSocket() != SOCKET_CONNECTION_CREATION_OK_RESULT) {
        usleep(100);
    }
    fpng::fpng_init();
}

void Client::requestOptimalPath(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
    int32_t coordinates[DATA_MAX_SIZE] = {0};
    coordinates[0] = x1;
    coordinates[1] = y1;
    coordinates[2] = x2;
    coordinates[3] = y2;

    // Request path from a server
    printf("Requesting optimal path for coordinates: %d %d %d %d.\n", coordinates[0], coordinates[1], coordinates[2], coordinates[3]);
    writeData(coordinates);

    // Wait for a response
    int32_t pathData[DATA_MAX_SIZE] = {0};
    int bytesRead = readData(pathData);

    if(bytesRead > 0) {
        auto coordinatesCount = pathData[0];
        if(coordinatesCount == ERROR_GETTING_PATH_RETURN_CODE) {
            std::cout<< "Path does not exist." << std::endl;
        }
        else {
            std::cout<< "Path received from the server." << std::endl;
            Path path;
            for(size_t i = 1; i <= coordinatesCount; i+=2) {
                path.push_back({pathData[i], pathData[i+1]});
            }
            visualizePathToPng(path);
        }
    }
    else {
        std::cout<< "Any path data received from the server." << std::endl;
    }
}

void Client::visualizePathToPng(const Path& path)
{
    if (path.empty()) {
        return;
    }

    const int width = 240;
    const int height = 320;

    std::vector<unsigned char> image(width * height * 3, 255);

    int x = path.front().first;
    int y = path.front().second;
    int r = 0;
    int g = 0;
    int b = 255;

    for(const auto& point : path) {
        x = point.first;
        y = point.second;
        int index = (y * width + x) * 3;
        image[index] = r;
        image[index + 1] = g;
        image[index + 2] = b;
    }
    // Visualise path to file
    std::string fileName("map.png");
    stbi_write_png(fileName.c_str(), width, height, 3, image.data(), 0);

    std::cout << "Path was visualized to the file " << fileName << std::endl;
}

int Client::createSocket()
{
    std::cout << "Creating socket." << std::endl;
    
    // Creating socket file descriptor
    if((mServerFD = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return SOCKET_CONNECTION_CREATION_ERROR_RESULT;
    }

    mAddress.sin_family = AF_INET;
    mAddress.sin_port = htons(PORT);

    // Converting IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &mAddress.sin_addr) <= 0) {
        return SOCKET_CONNECTION_CREATION_ERROR_RESULT;
    }

    // Connecting to the server
    if (connect(mServerFD, (struct sockaddr *)&mAddress, sizeof(mAddress)) < 0) {
        return SOCKET_CONNECTION_CREATION_ERROR_RESULT;
    }

    std::cout << "Socket connection estabilished to Server port " << PORT << "." << std::endl;
    return SOCKET_CONNECTION_CREATION_OK_RESULT;
}

void Client::writeData(int32_t* buffer)
{
    std::cout << "Writing data to the Server." << std::endl;

    // Sending data to the server
    send(mServerFD, buffer, DATA_MAX_SIZE, 0);

    std::cout<< "Data sent to the Server." << std::endl;
}

int Client::readData(int32_t* buffer)
{
    std::cout << "Waiting for data to read from the Server." << std::endl;

    // Reading data from the server
    int bytesRead = read(mServerFD, buffer, DATA_MAX_SIZE);

    printf("Data read by Client: %d bytes.\n", bytesRead);

    return bytesRead;
}

}