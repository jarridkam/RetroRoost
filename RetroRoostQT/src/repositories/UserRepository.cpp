#include "UserRepository.h"
#include "../core/DatabaseManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QDebug>


static inline User mapRowToUser(const QSqlQuery &row)
{
    return User(
        row.value("name").toString(),
        row.value("email").toString(),
        row.value("password").toString()
    );
}


QString UserRepository::hashPassword(const QString &plain)
{
    return QString(
        QCryptographicHash::hash(plain.toUtf8(), QCryptographicHash::Sha256).toHex()
    );
}

UserRepository::UserRepository(QObject *parent)
    : QObject(parent)
{
}

// ----------------------------------------------------------------------------
// CREATE USER
// ----------------------------------------------------------------------------
bool UserRepository::createUser(const User &user)
{
    QSqlDatabase db = DatabaseManager::connection(QStringLiteral("user"));
    if (!db.isOpen()) {
        qWarning() << "[users] Database not open!";
        return false;
    }

    QSqlQuery query(db);
    query.prepare(
        "INSERT INTO users (name, email, password) "
        "VALUES (:name, :email, :password) "
        "ON CONFLICT (email) DO NOTHING"
    );

    query.bindValue(":name", user.GetName());
    query.bindValue(":email", user.GetEmail());
    query.bindValue(":password", hashPassword(user.GetPassword()));

    if (!query.exec()) {
        qWarning().noquote() << "[users] Insert failed:"
                             << query.lastError().text();
        return false;
    }

    const bool success = (query.numRowsAffected() == 1);
    if (success)
        qDebug().noquote() << "[users] Insert succeeded for:" << user.GetName();
    else
        qDebug().noquote() << "[users] Insert skipped (duplicate email)";
    return success;
}

bool UserRepository::createUser(const QString &name,
                                const QString &email,
                                const QString &password)
{
    return createUser(User{name, email, password});
}

QList<User> UserRepository::getAllUsers()
{
    QList<User> users;
    QSqlDatabase db = DatabaseManager::connection(QStringLiteral("user"));
    if (!db.isOpen()) {
        qWarning() << "[users] Database not open!";
        return users;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT name, email, password FROM users")) {
        qWarning().noquote() << "[users] getAllUsers failed:"
                             << query.lastError().text();
        return users;
    }

    while (query.next())
        users.append(mapRowToUser(query));

    qDebug() << "[users] getAllUsers returned rows:" << users.size();
    return users;
}


// LOGIN VALIDATION
bool UserRepository::login(const QString &email,
                           const QString &password)
{
    auto foundUser = findUserByEmailAndPassword(email, password);
    if (foundUser.has_value()) {
        qDebug().noquote() << "[auth] Login successful for:" << foundUser->GetEmail();
        return true;
    }

    qWarning().noquote() << "[auth] Login failed for:" << email;
    return false;
}


std::optional<User> UserRepository::findUserByEmailAndPassword(
    const QString &email,
    const QString &password)
{
    QSqlDatabase db = DatabaseManager::connection(QStringLiteral("user"));
    if (!db.isOpen()) {
        qWarning() << "[users] Database not open!";
        return std::nullopt;
    }

    QSqlQuery query(db);
    query.prepare(
        "SELECT name, email, password FROM users "
        "WHERE email = :email AND password = :password"
    );

    query.bindValue(":email", email);
    query.bindValue(":password", hashPassword(password));

    if (!query.exec()) {
        qWarning().noquote() << "[users] Login query failed:"
                             << query.lastError().text();
        return std::nullopt;
    }

    if (query.next())
        return mapRowToUser(query);

    return std::nullopt;
}
