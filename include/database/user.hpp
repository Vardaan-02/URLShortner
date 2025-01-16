#ifndef USER_HPP
#define USER_HPP

#include <string>
#include <vector>

class User
{
private:
    std::string username;
    std::string email;
    std::string password;
    std::string salt;
    int free_trails_left;
    std::string session_token;
    std::vector<std::string> url_history_long;
    std::vector<std::string> url_history_short;

public:
    User(const std::string &username, const std::string &email, const std::string &password, const std::string &salt);
    User(const std::string &username, const std::string &email, const std::string &password, const std::string &salt, int free_trails_left, const std::string &session_token, const std::vector<std::string> &url_history_long = {}, const std::vector<std::string> &url_history_short = {});

    std::string getUsername() const;
    void setUsername(const std::string &username);

    std::string getEmail() const;
    void setEmail(const std::string &email);

    std::string getPassword() const;
    void setPassword(const std::string &password);

    std::string getSalt() const;
    void setSalt(const std::string &salt);

    std::vector<std::string> getUrlHistoryLong() const;
    void addUrlToHistoryLong(const std::string &long_url);

    std::vector<std::string> getUrlHistoryShort() const;
    void addUrlToHistoryShort(const std::string &short_url);

    int getFreeTrialsLeft() const;
    void decreaseFreeTrials();

    std::string getSessionToken() const;
    void setSessionToken(const std::string &token);
    void removeSessionToken();

    void serialize(const std::string &filename) const;
    static User deserialize(const std::string &filename, int line_number);

    static User fetchUserByEmail(const std::string &filename, const std::string &email);
    void deleteUser(const std::string &filename);
    static User fetchUserByRefreshToken(const std::string &filename, const std::string &sessionToken);

private:
    static std::string join(const std::vector<std::string> &vec, const std::string &delimiter);
    static std::vector<std::string> split(const std::string &str, const std::string &delimiter);
};

#endif 
