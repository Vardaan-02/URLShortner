#ifndef SHORT_URL_GENERATOR_HPP
#define SHORT_URL_GENERATOR_HPP

#include <string>

std::string generateHash(const std::string& url);
bool urlExists(const std::string& shortened, const std::string& filepath);
bool originalUrlExists(const std::string& original, const std::string& filepath);
std::string getOriginalUrlForShortened(const std::string& shortened, const std::string& filepath);
std::string getShortenedUrlForOriginal(const std::string& original, const std::string& filepath);
void storeUrlMapping(const std::string& shortened, const std::string& original, const std::string& filepath);
std::string shortenUrl(const std::string& originalUrl, const std::string& filepath);

#endif 