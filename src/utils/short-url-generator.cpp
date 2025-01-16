#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <openssl/sha.h>
#include <random>
#include <iomanip>
#include "utils/short-url-generator.hpp"

std::string generateHash(const std::string& url) {
    const int length = 8;
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

bool urlExists(const std::string& shortened, const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.substr(0, 8) == shortened) {
            return true;
        }
    }
    return false;
}

bool originalUrlExists(const std::string& original, const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.substr(9) == original || !line.substr(9).compare(original)) { 
            return true;
        }
    }
    return false;
}


std::string getOriginalUrlForShortened(const std::string& shortened, const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.substr(0, 8) == shortened) {
            return line.substr(9); 
        }
    }
    return "";
}

std::string getShortenedUrlForOriginal(const std::string& original, const std::string& filepath) {
    std::ifstream file(filepath);
    std::string line;
    
    while (std::getline(file, line)) {
        if (!line.substr(9).compare(original) || line.substr(9) ==  original) {
            return "http://localhost:8080/vardaan.ly/" + line.substr(0, 8);
        }
    }
    return "";
}

void storeUrlMapping(const std::string& shortened, const std::string& original, const std::string& filepath) {
    std::ofstream file(filepath, std::ios::app);
    file << shortened << " " << original << "\n";
}

std::string shortenUrl(const std::string& originalUrl, const std::string& filepath) {
    std::string existingShortenedUrl = getShortenedUrlForOriginal(originalUrl, filepath);
    if (!existingShortenedUrl.empty()) {
        return existingShortenedUrl;
    }

    std::string shortened = generateHash(originalUrl);
    
    while (urlExists(shortened, filepath)) {
        shortened = generateHash(originalUrl + std::to_string(rand()));
    }
    
    storeUrlMapping(shortened, originalUrl, filepath);

    return "http://localhost:8080/vardaan.ly/" + shortened;
}
