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
    vector<string> url_history_long;
    vector<string> url_history_short;
    int free_trails_left;
    string session_token;

public:
    User(const string &username, const string &email, const string &password);

    string getUsername() const;
    void setUsername(const string &username);

    string getEmail() const;
    void setEmail(const string &email);

    string getPassword() const;
    void setPassword(const string &password);

    vector<string> getUrlHistoryLong() const;
    void addUrlToHistoryLong(const string &long_url);

    vector<string> getUrlHistoryShort() const;
    void addUrlToHistoryShort(const string &short_url);

    int getFreeTrialsLeft() const;
    void decreaseFreeTrials();

    string getSessionToken() const;
    void setSessionToken(const string &token);
    void removeSessionToken(const string &token);
};

#endif
