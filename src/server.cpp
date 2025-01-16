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
#include <fcntl.h>
#include <cerrno>
#include "routes/auth/register.hpp"
#include "routes/auth/login.hpp"
#include "routes/user/get-user.hpp"
#include "routes/user/get-all-users.hpp"
#include "routes/url/url-shorten.hpp"
#include "routes/url/redirect.hpp"
#include "routes/404NotFound.hpp"
#include "utils/helper.hpp"

class Server
{
public:
    Server(int port) : port_(port) {}

    void start()
    {
        int sockfd = createSocket();
        bindSocket(sockfd);
        listenOnSocket(sockfd);

        std::cout << "Server started on Port : " << port_ << " (IPv6)" << std::endl;

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
        int sockfd = socket(AF_INET6, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            perror("Error opening socket");
            exit(1);
        }
        return sockfd;
    }

    void bindSocket(int sockfd)
    {
        struct sockaddr_in6 serverAddr = {};
        serverAddr.sin6_family = AF_INET6;
        serverAddr.sin6_addr = in6addr_any;
        serverAddr.sin6_port = htons(port_);

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
        struct sockaddr_in6 clientAddr = {};
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
        char buffer[4096];
        memset(buffer, 0, sizeof(buffer));

        int n = recv(clientSockfd, buffer, sizeof(buffer) - 1, 0);
        if (n < 0)
        {
            perror("Error on receiving");
            close(clientSockfd);
            return;
        }

        std::string request(buffer);
        std::istringstream requestStream(request);
        std::string method, path, version;
        std::string refreshToken;
        requestStream >> method >> path >> version;

        std::string body;
        std::string line;
        bool bodyFlag = false;
        while (std::getline(requestStream, line))
        {
            if (line.empty())
            {
                break;
            }
            if (bodyFlag)
            {
                body = line;
                bodyFlag = false;
            }
            if (line[0] == 13)
            {
                bodyFlag = true;
            }
            if(line.find("Cookie")!=std::string::npos){
                refreshToken = extractRefreshToken(line);
            }
        }

        if (path == "/register")
        {
            handleRegister(clientSockfd, method, path, body);
        }
        else if (path == "/login")
        {
            handleLogin(clientSockfd, method, path, body);
        }
        else if (path == "/get-user")
        {
            getUserByEmail(clientSockfd, method, path, body);
        }
        else if (path == "/get-all-users")
        {
            getAllUsers(clientSockfd, method, path, body,refreshToken);
        }
        else if (path == "/url-shortener")
        {
            shortenURL(clientSockfd, method, path, body,refreshToken);
        }
        else if(path.find("/vardaan.ly")!=std::string::npos){
            redirect(clientSockfd, method, path);
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
