#include "routes/auth/register.hpp"
#include "response.hpp"
#include "utils/helper.hpp"
#include "database/user.hpp"
#include "request-parser/auth/register.parser.hpp"
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <map>

void handleLogin(int clientSockfd, const std::string &method, const std::string &path, const std::string &body)
{
    if (method == "GET")
    {
        std::string body = readHtmlFile("/home/vardaan/low level coding/Web Server/public/login.html");
        std::string response = generateResponse(200, "OK", "text/html", body, {});
        sendResponse(clientSockfd, response);
        return;
    }
    else if (method == "POST")
    {
        std::map<std::string, std::string> parsedData = parseQueryString(body);

        if (parsedData.find("email") == parsedData.end() || parsedData.find("password") == parsedData.end())
        {
            std::string response = generateResponse(400, "Bad Request", "text/plain", "Missing email or password", {});
            sendResponse(clientSockfd, response);
            return;
        }

        if (!isValidEmail(parsedData["email"]))
        {
            std::string response = generateResponse(400, "Bad Request", "text/plain", "Invalid email format", {});
            sendResponse(clientSockfd, response);
            return;
        }

        try
        {
            User user = User::fetchUserByEmail("/home/vardaan/low level coding/Web Server/data/database.txt", parsedData["email"]);
            
            if (!verifyPassword(parsedData["password"], user.getSalt(), user.getPassword()))
            {
                std::string response = generateResponse(400, "Bad Request", "text/plain", "Incorrect email or password", {});
                sendResponse(clientSockfd, response);
                return;
            }
            std::string refreshToken = generateSecureToken(32);
            user.setSessionToken(refreshToken);

            user.deleteUser("/home/vardaan/low level coding/Web Server/data/database.txt");
            user.serialize("/home/vardaan/low level coding/Web Server/data/database.txt");

            std::string cookieHeader = "Set-Cookie: refreshToken=" + refreshToken + "; HttpOnly; Secure; Path=/; Max-Age=2592000"; // 30 days

            std::vector<std::string> headers = {cookieHeader};

            std::string response = generateResponse(200, "OK", "text/plain", "Login Successful",headers);
            sendResponse(clientSockfd, response);
        }
        catch (const std::runtime_error &e)
        {
            std::string response = generateResponse(400, "Bad Request", "text/plain", "Email not found", {});
            sendResponse(clientSockfd, response);
        }
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
