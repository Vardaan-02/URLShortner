
#ifndef GETALLUSERS_HPP
#define GETALLUSERS_HPP

#include <string>

void getAllUsers(int clientSockfd, const std::string &method, const std::string &path, const std::string &body,const std::string &refreshToken);

#endif
