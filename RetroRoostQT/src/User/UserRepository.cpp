#include "UserRepository.h"
#include "../DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

bool UserRepository::createUser(const QString &name, const QString &email) {
    QSqlQuery userQuery(DatabaseManager::connection("user"));
    userQuery.prepare("INSERT INTO users (name, email) VALUES (:name, :email)");
    userQuery.bindValue(":name", name);
    userQuery.bindValue(":email", email);

    qDebug() << "Inserting user:" << name << email;

    if (!userQuery.exec()) {
        qWarning() << "Insert failed:" << userQuery.lastError().text();
        return false;
    }

    qDebug() << "Insert succeeded for" << name;
    return true;
}

QList<User> UserRepository::getAllUsers() {
    QList<User> userList;
    QSqlQuery query(DatabaseManager::connection("user"));

    if (!query.exec("SELECT id, name, email FROM users")) {
        qWarning() << "getAllUsers failed:" << query.lastError().text();
        return userList;
    }

    int rowCount = 0;
    while (query.next()) {
        User u(
            query.value(1).toString(),
            query.value(2).toString());

        userList.append(u);
        rowCount++;
    }

    //qDebug() << "getAllUsers returned rows:" << rowCount;
    return userList;
}
