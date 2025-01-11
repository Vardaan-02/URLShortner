#include "routes/auth/register.hpp"
#include "response.hpp"
#include "utils/helper.hpp"
#include "request-parser/auth/register.parser.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>

void handleRegister(int clientSockfd, const std::string &method, const std::string &path, const std::string &body)
{
    if (method == "GET")
    {
        std::string body = readHtmlFile("/home/vardaan/low level coding/Web Server/public/register.html");
        std::string response = generateResponse(200, "OK", "text/html", body);
        sendResponse(clientSockfd, response);
    }
    else if (method == "POST")
    {
        std::map<std::string, std::string> parsedData = parseQueryString(body);
        for (const auto &pair : parsedData)
        {
            std::cout << pair.first << ": " << pair.second << std::endl;
        }
        std::string body = "Registration Completed";
        std::string response = generateResponse(200, "OK", "text", body);
        sendResponse(clientSockfd, response);
    }
    else
    {
        std::string body = "Unsupported HTTP method: " + method;
        std::string response = generateResponse(405, "Method Not Allowed", "text/plain", body);
        sendResponse(clientSockfd, response);
    }
}
