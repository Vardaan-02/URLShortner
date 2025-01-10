#include <iostream>
#include <array>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sstream>
#include <thread>
#include <cstring>
#include "routes/auth/register.hpp" 
#include "routes/404NotFound.hpp" 

class Server
{
public:
    Server(int port) : port_(port) {}

    void start()
    {
        int sockfd = createSocket();
        bindSocket(sockfd);
        listenOnSocket(sockfd);

        std::cout << "Server started on Port : " << port_ << std::endl;

        while (true)
        {
            int clientSockfd = acceptConnection(sockfd);
            std::thread clientThread(&Server::handleRequest, this, clientSockfd);
            clientThread.detach();
        }

        close(sockfd);
    }

private:
    int port_;

    int createSocket()
    {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("Error opening socket");
            exit(1);
        }
        return sockfd;
    }

    void bindSocket(int sockfd)
    {
        struct sockaddr_in serverAddr = {};
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port_);

        if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
        {
            perror("Binding failed");
            exit(1);
        }
    }

    void listenOnSocket(int sockfd)
    {
        if (listen(sockfd, 5) < 0)
        {
            perror("Error on listen");
            exit(1);
        }
    }

    int acceptConnection(int sockfd)
    {
        struct sockaddr_in clientAddr = {};
        socklen_t clientLen = sizeof(clientAddr);
        int newsockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);
        if (newsockfd < 0)
        {
            perror("Error on accept");
            exit(1);
        }
        return newsockfd;
    }

    void handleRequest(int clientSockfd)
    {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));
        int n = read(clientSockfd, buffer, sizeof(buffer) - 1);
        if (n < 0)
        {
            perror("Error on reading");
            close(clientSockfd);
            return;
        }

        std::string request(buffer);
        std::istringstream requestStream(request);
        std::string method, path, version;
        requestStream >> method >> path >> version;

        if (path == "/auth")
        {
            handleRegister(clientSockfd, method, path);
        }
        else
        {
            handle404NotFound(clientSockfd);
        }
    }
};

int main(int argc, char *argv[])
{
    int port = (argc < 2) ? 8080 : std::stoi(argv[1]);
    Server server(port);
    server.start();
    return 0;
}
