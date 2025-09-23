#include "DatabaseManager.h"
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

// static members must be defined once
QSqlDatabase DatabaseManager::userdb;
QSqlDatabase DatabaseManager::mediadb;

bool DatabaseManager::initUser() {
    userdb = QSqlDatabase::addDatabase("QSQLITE", "user");
    userdb.setDatabaseName("C:/Users/Jarrid/RetroRoost/userdb.sqlite");

    if (!userdb.open()) {
        qWarning() << "Failed to open user DB:" << userdb.lastError().text();
        return false;
    }
    qDebug() << "userdb opened at:" << userdb.databaseName();

    // Ensure table exists
    QSqlQuery q(userdb);
    if (!q.exec(
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL, "
        "email TEXT NOT NULL)"
    )) {
        qWarning() << "Failed to create users table:" << q.lastError().text();
        return false;
    }
    qDebug() << "users table ready";

    return true;
}

bool DatabaseManager::initMedia() {
    mediadb = QSqlDatabase::addDatabase("QSQLITE", "media");
    mediadb.setDatabaseName("C:/Users/Jarrid/RetroRoost/mediadb.sqlite");

    if (!mediadb.open()) {
        qWarning() << "Failed to open media DB:" << mediadb.lastError().text();
        return false;
    }
    qDebug() << "mediadb opened at:" << mediadb.databaseName();

    // Ensure table exists
    QSqlQuery q(mediadb);
    if (!q.exec(
        "CREATE TABLE IF NOT EXISTS media ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "title TEXT NOT NULL)"
    )) {
        qWarning() << "Failed to create media table:" << q.lastError().text();
        return false;
    }
    qDebug() << "media table ready";

    return true;
}

QSqlDatabase DatabaseManager::connection(const QString &which) {
    if (which == "media") return mediadb;
    return userdb;
}
