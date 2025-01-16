#include <iostream>
#include <vector>
#include <string>
#include "database/user.hpp"
#include "response.hpp"
#include "utils/helper.hpp"
#include "utils/short-url-generator.hpp"
#include "routes/url/url-shorten.hpp"
#include "request-parser/auth/register.parser.hpp"

using namespace std;

void shortenURL(int clientSockfd, const std::string &method, const std::string &path, const std::string &body, const std::string &refreshToken)
{
    if (method == "GET")
    {
        try
        {
            User user = User::fetchUserByRefreshToken("/home/vardaan/low level coding/Web Server/data/database.txt", refreshToken);

            std::string body = readHtmlFile("/home/vardaan/low level coding/Web Server/public/url-shorter.html");

            std::vector<string> longUrl = user.getUrlHistoryLong();
            std::vector<string> shortUrl = user.getUrlHistoryShort();

            std::string entry1 = "<tr><td>";
            std::string entry2 = "</td><td>";
            std::string entry3 = "</td></tr>";
            std::string replace;

            for (int i = 0; i < longUrl.size()-1; i++)
            {
                replace += entry1 + longUrl[i+1] + entry2 + shortUrl[i+1] + entry3;
            }

            body = replacePlaceholder(body, "{{data}}", replace);

            std::string response = generateResponse(200, "OK", "text/html", body, {});
            sendResponse(clientSockfd, response);
            return;
        }
        catch (const std::runtime_error &e)
        {
            std::string response = generateResponse(400, "Bad Request", "text/plain", "Invalid Refresh Token", {});
            sendResponse(clientSockfd, response);
        }
    }
    else if (method == "POST")
    {
        User user = User::fetchUserByRefreshToken("/home/vardaan/low level coding/Web Server/data/database.txt", refreshToken);

        std::map<std::string, std::string> parsedData = parseQueryString(body);

        try
        {
            if (parsedData.find("url") == parsedData.end())
            {
                std::string response = generateResponse(400, "Bad Request", "text/plain", "Missing url", {});
                sendResponse(clientSockfd, response);
                return;
            }

            std::string response;
            std::string url = shortenUrl(parsedData["url"], "/home/vardaan/low level coding/Web Server/data/url-mapping.txt");

            user.deleteUser("/home/vardaan/low level coding/Web Server/data/database.txt");
            user.addUrlToHistoryLong(parsedData["url"]);
            user.addUrlToHistoryShort(url);
            user.serialize("/home/vardaan/low level coding/Web Server/data/database.txt");

            response = generateResponse(200, "OK", "text", url, {});
            sendResponse(clientSockfd, response);
            return;
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
