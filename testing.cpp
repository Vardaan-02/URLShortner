#include <bits/stdc++.h>

std::string generateHash() {
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

int main() {
    std::cout << generateHash() << std::endl;
    std::cout << generateHash() << std::endl;
    std::cout << generateHash() << std::endl;

    std::string line = "a78ab25c https://www.youtube.com/";
    std::string original = "https://www.amazon.in/";

    if (!line.substr(9).compare(original) || line.substr(9) ==  original) {
        std::cout << line << " " << line.substr(9) << " " << original << std::endl;
    }

    std::cout << "Hello World" << std::endl;
    return 0;
}
