#include "routes/404NotFound.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>

void handle404NotFound(int clientSockfd)
{
    std::string body = "<html><body><h1>404 Not Found!</h1></body></html>";
    std::string response =
        "HTTP/1.1 404 Not Found\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n" + body;

    if (write(clientSockfd, response.c_str(), response.size()) < 0)
    {
        perror("Error writing to socket");
    }

    shutdown(clientSockfd, SHUT_RDWR);
    close(clientSockfd);
}
