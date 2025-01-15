
#ifndef GETUSER_HPP
#define GETUSER_HPP

#include <string>

void getUserByEmail(int clientSockfd, const std::string &method, const std::string &path, const std::string &body);

#endif
