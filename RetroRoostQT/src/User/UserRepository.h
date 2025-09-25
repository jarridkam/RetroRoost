#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QObject>
#include <QList>
#include "User.h"

class UserRepository : public QObject {
    Q_OBJECT
public:
    explicit UserRepository(QObject *parent = nullptr);


    Q_INVOKABLE static bool createUser(
        const QString &name,
        const QString &email,
        const QString &password
        );


    static bool createUser(const User &user);
    static std::optional<User> findUserByEmailAndPassword(
        const QString& email,
        const QString& password
    );


    QList<User> getAllUsers();
};

#endif // USERREPOSITORY_H
