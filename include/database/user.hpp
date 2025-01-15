#ifndef USER_HPP
#define USER_HPP

#include <vector>
#include <string>
using namespace std;

class User
{
private:
    string username;
    string email;
    string password;
    string salt;
    vector<string> url_history_long;
    vector<string> url_history_short;
    int free_trails_left;
    string session_token;

public:
    User(const string &username, const string &email, const string &password, const string &salt);
    User(const string &username, const string &email, const string &password, const string &salt, int free_trails_left, const string &session_token);

    string getUsername() const;
    void setUsername(const string &username);

    string getEmail() const;
    void setEmail(const string &email);

    string getPassword() const;
    void setPassword(const string &password);

    string getSalt() const;
    void setSalt(const string &salt);

    vector<string> getUrlHistoryLong() const;
    void addUrlToHistoryLong(const string &long_url);

    vector<string> getUrlHistoryShort() const;
    void addUrlToHistoryShort(const string &short_url);

    int getFreeTrialsLeft() const;
    void decreaseFreeTrials();

    string getSessionToken() const;
    void setSessionToken(const string &token);
    void removeSessionToken();

    void serialize(const string &filename) const;
    static User deserialize(const string &filename, int line_number);

    static User fetchUserByEmail(const std::string &filename, const std::string &email);
    void deleteUser(const std::string &filename);

    static User fetchUserByRefreshToken(const std::string &filename, const std::string &email);
};

#endif
