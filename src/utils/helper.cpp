#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <random>
#include <openssl/sha.h>
#include <iomanip>

std::string readHtmlFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Could not open the file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

void printCurrentWorkingDirectory()
{
    char buf[1024];
    if (getcwd(buf, sizeof(buf)) != NULL)
    {
        std::cout << "Current working directory: " << buf << std::endl;
    }
    else
    {
        perror("getcwd");
    }
}

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" ");
    size_t last = str.find_last_not_of(" ");

    if (first == std::string::npos || last == std::string::npos) {
        return "";
    }

    return str.substr(first, last - first + 1);
}

bool isValidEmail(const std::string& email) {
    const std::regex emailPattern(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");
    return std::regex_match(email, emailPattern);
}

std::string generateSalt(size_t length) {
    const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    std::string salt;
    std::default_random_engine rng(std::random_device{}());
    std::uniform_int_distribution<size_t> dist(0, sizeof(charset) - 2);
    for (size_t i = 0; i < length; ++i) {
        salt += charset[dist(rng)];
    }
    return salt;
}

std::string hashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    
    std::ostringstream os;
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        os << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return os.str();
}

bool verifyPassword(const std::string& enteredPassword, const std::string& storedSalt, const std::string& storedHash) {
    std::string saltedPassword = storedSalt + enteredPassword;
    std::string computedHash = hashPassword(saltedPassword);
    return computedHash.compare(storedHash);
}

std::string generateSecureToken(size_t length = 32)
{
    std::random_device rd;
    std::mt19937_64 engine(rd());
    std::uniform_int_distribution<int> dist(0, 15);

    std::ostringstream token;
    for (size_t i = 0; i < length; ++i)
    {
        token << std::hex << dist(engine);
    }

    return token.str();
}

std::string extractRefreshToken(const std::string& cookie) {
    const std::string tokenKey = "refreshToken=";
    size_t pos = cookie.find(tokenKey);
    if (pos != std::string::npos) {
        pos += tokenKey.length(); 
        size_t end = cookie.find(';', pos);
        return cookie.substr(pos, end - pos);
    }
    return "";
}