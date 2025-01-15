#ifndef LOGIN_HPP
#define LOGIN_HPP

#include <string>

void handleLogin(int clientSockfd, const std::string &method, const std::string &path,const std::string &body);

#endif 
