#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <regex>

std::map<std::string, std::string> parseKeyValuePairs(const std::string &input)
{
    std::map<std::string, std::string> data;
    std::regex rgx("\"([^\"]+)\": \"([^\"]+)\"");
    std::smatch match;
    std::string::const_iterator searchStart(input.cbegin());
    while (std::regex_search(searchStart, input.cend(), match, rgx))
    {
        std::string key = match[1].str();
        std::string value = match[2].str();
        data[key] = value;
        searchStart = match.suffix().first;
    }
    return data;
}

std::string urlDecode(const std::string& encoded) {
    std::string decoded = "";
    for (size_t i = 0; i < encoded.length(); i++) {
        if (encoded[i] == '%' && i + 2 < encoded.length() && std::isxdigit(encoded[i + 1]) && std::isxdigit(encoded[i + 2])) {
            std::string hex = encoded.substr(i + 1, 2);
            char decodedChar = static_cast<char>(std::stoi(hex, nullptr, 16));
            decoded += decodedChar;
            i += 2;
        } else {
            decoded += encoded[i];
        }
    }
    return decoded;
}

std::map<std::string, std::string> parseQueryString(const std::string& query) {
    std::map<std::string, std::string> params;
    std::stringstream ss(query);
    std::string item;

    while (std::getline(ss, item, '&')) {
        size_t pos = item.find('=');
        if (pos != std::string::npos) {
            std::string key = urlDecode(item.substr(0, pos));
            std::string value = urlDecode(item.substr(pos + 1));
            params[key] = value;
        }
    }

    return params;
}