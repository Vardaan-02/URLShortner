#include <iostream>
#include <vector>
#include <string>
#include "database/user.hpp"
#include "response.hpp"
#include "request-parser/auth/register.parser.hpp"
#include "utils/helper.hpp"

using namespace std;

void getUserByEmail(int clientSockfd, const std::string &method, const std::string &path, const std::string &body)
{
    if (method == "POST")
    {
        std::map<std::string, std::string> parsedData = parseQueryString(body);

        if (parsedData.find("email") == parsedData.end())
        {
            std::string errorResponse = generateResponse(400, "Bad Request", "text/html", "<html><body><h1>Error: Missing 'email'</h1></body></html>",{});
            sendResponse(clientSockfd, errorResponse);
            return;
        }

        std::string email = parsedData["email"];
        User user = User::fetchUserByEmail("/home/vardaan/low level coding/Web Server/data/database.txt", email);

        std::string body = "<html><body><h1>All Users</h1><ul>";
        body += "<li>" + user.getUsername() + "-" + user.getEmail() + "</li>";
        body += "</ul></body></html>";

        std::string response = generateResponse(200, "OK", "text/html", body,{});
        sendResponse(clientSockfd, response);
    }
    else
    {
        std::string body = "Unsupported HTTP method: " + method;
        std::string response = generateResponse(405, "Method Not Allowed", "text/plain", body,{});
        sendResponse(clientSockfd, response);
    }
}