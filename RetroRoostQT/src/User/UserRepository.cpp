#include "UserRepository.h"
#include "../DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool UserRepository::createUser(const QString &name, const QString &email, const QString &password) {
    QSqlQuery q(DatabaseManager::connection("user"));
    q.prepare("INSERT OR IGNORE INTO users (name, email, password) "
              "VALUES (:name, :email, :password)");
    q.bindValue(":name", name);
    q.bindValue(":email", email);
    q.bindValue(":password", password);

    qDebug() << "Inserting user:" << name << email << password;

    if (!q.exec()) {
        qWarning() << "Insert failed:" << q.lastError().text();
        return false;
    }

    const bool inserted = (q.numRowsAffected() == 1);
    if (!inserted) qDebug() << "Skipped insert: username/email already exists";
    else qDebug() << "Insert succeeded for" << name;
    return inserted;
}

QList<User> UserRepository::getAllUsers() {
    QList<User> out;
    QSqlQuery q(DatabaseManager::connection("user"));

    if (!q.exec("SELECT name, email, password FROM users")) {
        qWarning() << "getAllUsers failed:" << q.lastError().text();
        return out;
    }

    while (q.next()) {
        out.append(
            User
            (
            q.value(0).toString(),   // name
            q.value(1).toString(),   // email
            q.value(2).toString())); // password
    }
    return out;
}


