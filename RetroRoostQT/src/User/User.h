#ifndef RETROROOSTQT_USER_H
#define RETROROOSTQT_USER_H

#include <utility>

class User {
private:
    QString userName;
    QString userPassword;

public:
    User() = default;
    User(QString _userName, QString _userPassword)
        : userName(std::move(_userName)), userPassword(std::move(_userPassword)) {}

    void SetName(const QString& _userName) { userName = _userName; }
    void SetPassword(const QString& _userPassword) { userPassword = _userPassword; }

    [[nodiscard]] const QString& GetName() const { return userName; }
    [[nodiscard]] const QString& GetPassword() const { return userPassword; }
};


#endif // RETROROOSTQT_USER_H