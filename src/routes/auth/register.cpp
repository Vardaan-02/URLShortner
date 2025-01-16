#include "routes/auth/register.hpp"
#include "response.hpp"
#include "utils/helper.hpp"
#include "database/user.hpp"
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
        std::string response = generateResponse(200, "OK", "text/html", body,{});
        sendResponse(clientSockfd, response);
    }
    else if (method == "POST")
    {
        std::map<std::string, std::string> parsedData = parseQueryString(body);

        if (parsedData.find("email") == parsedData.end() || parsedData.find("password") == parsedData.end() || parsedData.find("confirmPassword") == parsedData.end() || parsedData.find("username") == parsedData.end())
        {
            std::string response = generateResponse(400, "Bad Request", "text/plain", "Missing email or password",{});
            sendResponse(clientSockfd, response);
            return;
        }

        std::string response;
        if (!isValidEmail(parsedData["email"]))
        {
            response = generateResponse(400, "Bad Request", "text", "Email not valid",{});
            sendResponse(clientSockfd, response);
        }
        if (parsedData["password"].compare(parsedData["confirmPassword"]))
        {
            response = generateResponse(400, "Bad Request", "text", "Password does not match Confirm Password",{});
            sendResponse(clientSockfd, response);
        }
        std::string salt = generateSalt(16);
        std::string saltedPassword = salt + parsedData["password"];
        std::string hashedPassword = hashPassword(saltedPassword);
        User user(parsedData["username"], parsedData["email"], hashedPassword, salt);
        user.serialize("/home/vardaan/low level coding/Web Server/data/database.txt");
        response = generateResponse(200, "OK", "text", "Registration Successful",{});
        sendResponse(clientSockfd, response);
    }
    else
    {
        std::string body = "Unsupported HTTP method: " + method;
        std::string response = generateResponse(405, "Method Not Allowed", "text/plain", body,{});
        sendResponse(clientSockfd, response);
    }
}
