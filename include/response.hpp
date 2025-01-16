#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>
#include <vector>

std::string generateResponse(int statusCode, const std::string &statusMessage,
                             const std::string &contentType, const std::string &body,
                             const std::vector<std::string> &customHeaders = {});

std::string redirectResponse(int statusCode, const std::string &statusMessage,
                             const std::string &contentType, const std::string &body,
                             const std::vector<std::pair<std::string, std::string>> &customHeaders);

void sendResponse(int clientSockfd, const std::string &response);

#endif
