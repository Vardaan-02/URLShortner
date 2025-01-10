#include "routes/auth/register.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>

void handleRegister(int clientSockfd, const std::string &method, const std::string &path)
{
    if (method == "GET")
    {
        std::string body = "<html><body><h1>Hello World!</h1></body></html>";
        std::string response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "Content-Length: " + std::to_string(body.size()) + "\r\n"
            "\r\n" +
            body;

        if (write(clientSockfd, response.c_str(), response.size()) < 0)
        {
            perror("Error writing to socket");
        }

        shutdown(clientSockfd, SHUT_RDWR);
        close(clientSockfd);
    }
    else
    {
        std::cerr << "Unsupported HTTP method: " << method << std::endl;
        shutdown(clientSockfd, SHUT_RDWR);
        close(clientSockfd);
    }
}
