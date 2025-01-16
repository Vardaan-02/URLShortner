#ifndef REDIRECT_HPP
#define REDIRECT_HPP

#include <string>

void redirect(int clientSockfd, const std::string &method, const std::string &path);

#endif
