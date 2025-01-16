#ifndef URL_SHORTNER_HPP
#define URL_SHORTNER_HPP

#include <string>

void shortenURL(int clientSockfd, const std::string &method, const std::string &path, const std::string &body, const std::string &refreshToken);

#endif
