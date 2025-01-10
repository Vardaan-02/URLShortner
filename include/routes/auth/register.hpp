#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <string>

void handleRegister(int clientSockfd, const std::string &method, const std::string &path);

#endif 
