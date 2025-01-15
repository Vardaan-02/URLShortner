#ifndef PRINT_CURRENT_WORKING_DIRECTORY_HPP
#define PRINT_CURRENT_WORKING_DIRECTORY_HPP

#include <string>

void printCurrentWorkingDirectory();

std::string readHtmlFile(const std::string &filePath);

std::string trim(const std::string &str);

bool isValidEmail(const std::string &email);

std::string generateSalt(size_t length);
std::string generateSecureToken(size_t length);

std::string hashPassword(const std::string &password);

bool verifyPassword(const std::string &enteredPassword, const std::string &storedSalt, const std::string &storedHash);

std::string extractRefreshToken(const std::string& cookie);

#endif
