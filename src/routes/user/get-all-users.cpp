#include <iostream>
#include <vector>
#include <string>
#include "database/user.hpp"
#include "response.hpp"
#include "utils/helper.hpp"

using namespace std;

void getAllUsers(int clientSockfd, const std::string &method, const std::string &path, const std::string &body, const std::string &refreshToken)
{
    if (method == "GET")
    {
        std::vector<User> users;
        int line = 0;

        while (true)
        {
            try
            {
                users.push_back(User::deserialize("/home/vardaan/low level coding/Web Server/data/database.txt", line++));
            }
            catch (const std::runtime_error &e)
            {
                break;
            }
        }
        try
        {
            User user = User::fetchUserByRefreshToken("/home/vardaan/low level coding/Web Server/data/database.txt", refreshToken);

            std::string body = "<html><body><h1>All Users</h1><ul>";
            for (const auto &user : users)
            {
                body += "<li>" + user.getUsername() + " - " + user.getEmail() + "</li>";
            }
            body += "</ul></body></html>";

            std::string response = generateResponse(200, "OK", "text/html", body, {});
            sendResponse(clientSockfd, response);
        }
        catch (const std::runtime_error &e)
        {
            std::string response = generateResponse(400, "Bad Request", "text/plain", "Invalid Refresh Token", {});
            sendResponse(clientSockfd, response);
        }
    }
    else
    {
        std::string body = "Unsupported HTTP method: " + method;
        std::string response = generateResponse(405, "Method Not Allowed", "text/plain", body, {});
        sendResponse(clientSockfd, response);
    }
}
