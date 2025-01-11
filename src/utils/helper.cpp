#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>

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