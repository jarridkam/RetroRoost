#ifndef RETROROOSTQT_USER_H
#define RETROROOSTQT_USER_H

#include <utility>

class User {
private:
    QString userName;
    QString userEmail;
    QString userPassword;

public:
    User() = default;
    User(QString _userName,  QString _userEmail, QString _userPassword):
    userName(std::move(_userName)),
    userEmail(std::move(_userEmail)),
    userPassword(std::move(_userPassword)) {}

    void SetName(const QString& _userName) { userName = _userName; }
    void SetEmail(const QString& _userEmail) { userEmail = _userEmail; }
    void SetPassword(const QString& _userPassword) { userPassword = _userPassword; }

    [[nodiscard]] const QString& GetName() const { return userName; }
    [[nodiscard]] const QString& GetEmail() const { return userEmail; }
    [[nodiscard]] const QString& GetPassword() const { return userPassword; }
};


#endif // RETROROOSTQT_USER_H