#include "UserRepository.h"
#include "../DatabaseManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>


static inline User mapRowToUser(const QSqlQuery &row)
{
    return User(
        row.value(0).toString(), // name
        row.value(1).toString(), // email
        row.value(2).toString()  // password
    );
}

UserRepository::UserRepository(QObject *parent)
    : QObject(parent)
{
}

bool UserRepository::createUser(const User &user)
{
    QSqlDatabase db = DatabaseManager::connection(QStringLiteral("user"));
    if (!db.isOpen()) {
        qWarning() << "[users] Database not open!";
        return false;
    }

    QSqlQuery insertUserQuery(db);
    insertUserQuery.prepare(
        "INSERT OR IGNORE INTO users (name, email, password) "
        "VALUES (:name, :email, :password)"
    );
    insertUserQuery.bindValue(":name",     user.GetName());
    insertUserQuery.bindValue(":email",    user.GetEmail());
    insertUserQuery.bindValue(":password", user.GetPassword());

    qDebug().noquote()
        << "[users] Insert attempt:"
        << "name="  << user.GetName()
        << "email=" << user.GetEmail()
        << "password=****";

    if (!insertUserQuery.exec()) {
        qWarning().noquote() << "[users] Insert failed:"
                             << insertUserQuery.lastError().text();
        return false;
    }

    const bool rowInserted = (insertUserQuery.numRowsAffected() == 1);
    if (rowInserted) {
        qDebug().noquote() << "[users] Insert succeeded for"
                           << user.GetName();
    } else {
        qDebug() << "[users] Insert skipped (duplicate name/email)";
    }
    return rowInserted;
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

    QSqlQuery selectAllQuery(db);
    if (!selectAllQuery.exec("SELECT name, email, password FROM users")) {
        qWarning().noquote() << "[users] getAllUsers failed:"
                             << selectAllQuery.lastError().text();
        return users;
    }

    users.reserve(32);
    while (selectAllQuery.next()) {
        users.append(mapRowToUser(selectAllQuery));
    }

    qDebug() << "[users] getAllUsers returned rows:" << users.size();
    return users;
}
