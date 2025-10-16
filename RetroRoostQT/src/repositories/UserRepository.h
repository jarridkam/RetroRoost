#pragma once

#include <QObject>
#include <QList>
#include <optional>
#include "../core/models/User.h"

class UserRepository : public QObject
{
    Q_OBJECT
public:
    explicit UserRepository(QObject *parent = nullptr);

    // --- QML-callable methods ---
    Q_INVOKABLE bool createUser(const QString &name,
                                const QString &email,
                                const QString &password);

    Q_INVOKABLE QList<User> getAllUsers();

    Q_INVOKABLE bool login(const QString &email,
                           const QString &password);


    std::optional<User> findUserByEmailAndPassword(const QString &email,
                                                   const QString &password);

private:
    bool createUser(const User &user);
    static QString hashPassword(const QString &plain);
};
