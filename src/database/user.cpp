#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include "database/user.hpp"

using namespace std;

User::User(const string &username, const string &email, const string &password, const string &salt)
    : username(username), email(email), password(password), salt(salt), free_trails_left(3) {}

User::User(const string &username, const string &email, const string &password, const string &salt, int free_trails_left, const string &session_token)
    : username(username), email(email), password(password), salt(salt), free_trails_left(free_trails_left), session_token(session_token) {}

string User::getUsername() const { return username; }
void User::setUsername(const string &username) { this->username = username; }

string User::getEmail() const { return email; }
void User::setEmail(const string &email) { this->email = email; }

string User::getPassword() const { return password; }
void User::setPassword(const string &password) { this->password = password; }

string User::getSalt() const { return salt; }
void User::setSalt(const string &salt) { this->salt = salt; }

vector<string> User::getUrlHistoryLong() const { return url_history_long; }
void User::addUrlToHistoryLong(const string &long_url) { url_history_long.push_back(long_url); }

vector<string> User::getUrlHistoryShort() const { return url_history_short; }
void User::addUrlToHistoryShort(const string &short_url) { url_history_short.push_back(short_url); }

int User::getFreeTrialsLeft() const { return free_trails_left; }
void User::decreaseFreeTrials()
{
    if (free_trails_left > 0)
        free_trails_left--;
}

string User::getSessionToken() const { return session_token; }
void User::setSessionToken(const string &token) { this->session_token = token; }
void User::removeSessionToken() { this->session_token = ""; }

void User::serialize(const string &filename) const
{
    ofstream file(filename, ios::app);
    if (file.is_open())
    {
        file << username << ","
             << email << ","
             << password << ","
             << salt << ","
             << free_trails_left << ","
             << session_token << "\n";
        file.close();
    }
    else
    {
        cerr << "Error opening file for serialization!" << endl;
    }
}

User User::deserialize(const string &filename, int line_number)
{
    ifstream file(filename);
    string line;
    int current_line = 0;

    while (getline(file, line))
    {
        if (current_line == line_number)
        {
            stringstream ss(line);
            string username, email, password, session_token, salt;
            int free_trails_left;

            getline(ss, username, ',');
            getline(ss, email, ',');
            getline(ss, password, ',');
            getline(ss, salt, ',');
            ss >> free_trails_left;
            ss.ignore(1, ',');
            getline(ss, session_token, ',');

            file.close();
            return User(username, email, password, salt, free_trails_left, session_token);
        }
        current_line++;
    }

    file.close();
    throw runtime_error("User not found at line number " + to_string(line_number));
}

User User::fetchUserByEmail(const std::string &filename, const std::string &email)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    int current_line = 0;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string username, user_email, password, session_token, salt;
        int free_trails_left;

        std::getline(ss, username, ',');
        std::getline(ss, user_email, ',');
        std::getline(ss, password, ',');
        std::getline(ss, salt, ',');
        ss >> free_trails_left;
        ss.ignore(1, ',');
        std::getline(ss, session_token, ',');

        if (user_email.compare(email) || user_email == email)
        {
            file.close();
            return User(username, user_email, password, salt, free_trails_left, session_token);
        }
        current_line++;
    }
    file.close();
    throw std::runtime_error("User with email " + email + " not found.");
}

void User::deleteUser(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line))
    {
        lines.push_back(line);
    }

    file.close();

    auto it = std::remove_if(lines.begin(), lines.end(), [&](const std::string &entry)
                             {
        std::stringstream ss(entry);
        std::string username, user_email, password, session_token, salt;
        int free_trails_left;
        
        std::getline(ss, username, ',');
        std::getline(ss, user_email, ',');
        std::getline(ss, password, ',');
        std::getline(ss, salt, ',');
        ss >> free_trails_left;
        ss.ignore(1, ',');
        std::getline(ss, session_token, ',');
        
        return user_email == email || user_email.compare(email); });

    if (it == lines.end())
    {
        throw std::runtime_error("User with email " + email + " not found.");
    }

    lines.erase(it, lines.end());

    std::ofstream out_file(filename, std::ios::trunc);
    if (!out_file.is_open())
    {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    for (const auto &entry : lines)
    {
        out_file << entry << std::endl;
    }

    out_file.close();
}

User User::fetchUserByRefreshToken(const std::string &filename, const std::string &sessionToken)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string line;
    int current_line = 0;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string user_username, user_email, user_password, user_session_token, user_salt;
        int free_trails_left;

        std::getline(ss, user_username, ',');
        std::getline(ss, user_email, ',');
        std::getline(ss, user_password, ',');
        std::getline(ss, user_salt, ',');
        ss >> free_trails_left;
        ss.ignore(1, ',');
        std::getline(ss, user_session_token, ',');

        if (user_session_token == sessionToken || user_session_token.compare(sessionToken))
        {
            file.close();
            return User(user_username, user_email, user_password, user_salt, free_trails_left, user_session_token);
        }
        current_line++;
    }

    file.close();
    throw std::runtime_error("Invalid Session Token");
}