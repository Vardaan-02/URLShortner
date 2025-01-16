#include "response.hpp"
#include "routes/url/redirect.hpp"
#include "utils/short-url-generator.hpp"
#include <sys/socket.h>
#include <iostream>
#include <string>

void redirect(int clientSockfd, const std::string &method, const std::string &path)
{
    if (method == "GET")
    {
        std::string code = path.substr(path.find("/vardaan.ly/") + 12);
        std::string original = getOriginalUrlForShortened(code,"/home/vardaan/low level coding/Web Server/data/url-mapping.txt");
        
        std::string response = redirectResponse(301, "Found", "text", "", {{"Location", original}});
        sendResponse(clientSockfd, response);
        return;
    }
    else
    {
        std::string body = "Unsupported HTTP method: " + method;
        std::string response = generateResponse(405, "Method Not Allowed", "text/plain", body, {});
        sendResponse(clientSockfd, response);
        return;
    }
}
