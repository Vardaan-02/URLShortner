#ifndef PARSE_DATA_HPP
#define PARSE_DATA_HPP

#include <iostream>
#include <map>
#include <sstream>
#include <string>

std::map<std::string, std::string> parseKeyValuePairs(const std::string &input);

std::string urlDecode(const std::string &encoded);

std::map<std::string, std::string> parseQueryString(const std::string &query);

#endif
