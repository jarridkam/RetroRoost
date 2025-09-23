#ifndef RETROROOSTQT_USER_H
#define RETROROOSTQT_USER_H

#include <string>
#include <iostream>
#include <utility>

class User {
private:
    std::string userName;
    std::string userPassword;

public:
    User(std::string  _userName, std::string  _userPassword)
        : userName(std::move(_userName)), userPassword(std::move(_userPassword)) {}

    ~User() = default;

    [[nodiscard]] const std::string& GetName() const { return userName; }
    [[nodiscard]] const std::string& GetPassword() const { return userPassword; }
};

#endif // RETROROOSTQT_USER_H