#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>

// Function to decode URL-encoded characters
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

int main() {
    std::string query = "username=DysfunctionalDegenerate&email=vardaanpahwa02%40gamil.com&password=asdf&confirmPassword=asdf";
    
    std::map<std::string, std::string> parsedParams = parseQueryString(query);

    // Display the parsed map with decoded values
    for (const auto& param : parsedParams) {
        std::cout << param.first << " = " << param.second << std::endl;
    }

    return 0;
}
