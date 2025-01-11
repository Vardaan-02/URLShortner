#include "response.hpp"
#include <unistd.h>
#include <string>
#include <iostream>
#include <sys/socket.h>

std::string generateResponse(int statusCode, const std::string &statusMessage,
                             const std::string &contentType, const std::string &body)
{
    std::string response =
        "HTTP/1.1 " + std::to_string(statusCode) + " " + statusMessage + "\r\n" +
        "Content-Type: " + contentType + "\r\n" +
        "Connection: close\r\n" +
        "Content-Length: " + std::to_string(body.size()) + "\r\n" +
        "\r\n" +
        body;
    return response;
}

void sendResponse(int clientSockfd, const std::string &response)
{
    if (write(clientSockfd, response.c_str(), response.size()) < 0)
    {
        perror("Error writing to socket");
    }
    shutdown(clientSockfd, SHUT_RDWR);
    close(clientSockfd);
}
