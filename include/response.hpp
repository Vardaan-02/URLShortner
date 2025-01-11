#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <string>

std::string generateResponse(int statusCode, const std::string &statusMessage,
                             const std::string &contentType, const std::string &body);

void sendResponse(int clientSockfd, const std::string &response);

#endif
