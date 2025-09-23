// UserDatabase.cpp
#include "UserDatabase.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

UserDatabase::UserDatabase(const QString& dbPath)
    : dbPath(dbPath) {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);
}

UserDatabase::~UserDatabase() {
    close();
}

bool UserDatabase::open() {
    if (!db.open()) {
        qDebug() << "Error: Cannot open database -" << db.lastError().text();
        return false;
    }
    return initTables();
}

void UserDatabase::close() {if (db.isOpen()) {db.close();}}

bool UserDatabase::initTables() {
    QSqlQuery query;
    QString createTable =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "username TEXT UNIQUE NOT NULL, "
        "password TEXT NOT NULL)";
    if (!query.exec(createTable)) {
        qDebug() << "Failed to create table:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserDatabase::addUser(const QString& username, const QString& password) {
    if (userExists(username)) {
        qDebug() << "User already exists:" << username;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO users (username, password) "
                  "VALUES (:username, :password)");
    query.bindValue(":username", username);
    query.bindValue(":password", password);

    if (!query.exec()) {
        qDebug() << "Failed to insert user:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserDatabase::userExists(const QString& username) {
    QSqlQuery query;
    query.prepare("SELECT id FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "User check failed:" << query.lastError().text();
        return false;
    }
    return query.next();
}

bool UserDatabase::checkLogin(const QString& username, const QString& password) {
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        return (storedPassword == password);
    }
    return false;
}
