#include "DatabaseManager.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

// ---- static members ---------------------------------------------------------
QSqlDatabase DatabaseManager::s_userDb;
QSqlDatabase DatabaseManager::s_mediaDb;

// ---- helpers ---------------------------------------------------------------

static QString resolveAppDataDirectory()
{
    // Optional override for debugging or portable installs
    QString base = qEnvironmentVariable("RETRO_ROOST_DATA_DIR");
    if (base.isEmpty()) {
        if (QCoreApplication::organizationName().isEmpty())
            QCoreApplication::setOrganizationName("RetroRoost");
        if (QCoreApplication::applicationName().isEmpty())
            QCoreApplication::setApplicationName("RetroRoostQT");

        // Win:  %LOCALAPPDATA%/RetroRoost/RetroRoostQT
        // mac:  ~/Library/Application Support/RetroRoost/RetroRoostQT
        // linux:~/.local/share/RetroRoost/RetroRoostQT
        base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        if (base.isEmpty())
            base = QDir::homePath() + "/RetroRoost";
    }

    QDir dir(base);
    if (!dir.exists())
        dir.mkpath(".");

    return dir.absolutePath();
}

static QString dataFilePath(const QString& fileName)
{
    return QDir(resolveAppDataDirectory()).filePath(fileName);
}

static bool ensureUsersTableAndIndexes(QSqlDatabase& db)
{
    // 1) Ensure base table exists
    QSqlQuery createUsersTable(db);
    const char* createSql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  name TEXT NOT NULL,"
        "  email TEXT NOT NULL"
        ")";
    if (!createUsersTable.exec(createSql)) {
        qWarning() << "[users] create failed:" << createUsersTable.lastError().text();
        return false;
    }

    // 2) Check for 'password' column and add if missing
    QSqlQuery tableInfoQuery(db);
    if (!tableInfoQuery.exec("PRAGMA table_info(users)")) {
        qWarning() << "[users] PRAGMA table_info failed:" << tableInfoQuery.lastError().text();
        return false;
    }

    bool hasPasswordColumn = false;
    while (tableInfoQuery.next()) {
        // PRAGMA table_info: (cid, name, type, notnull, dflt_value, pk)
        if (tableInfoQuery.value(1).toString() == QLatin1String("password")) {
            hasPasswordColumn = true;
            break;
        }
    }

    if (!hasPasswordColumn) {
        QSqlQuery addPasswordColumn(db);
        if (!addPasswordColumn.exec("ALTER TABLE users ADD COLUMN password TEXT NOT NULL DEFAULT ''")) {
            qWarning() << "[users] add password column failed:" << addPasswordColumn.lastError().text();
            return false;
        }
        qDebug() << "[users] Added password column";
    }

    // 3) Create unique indexes (idempotent)
    QSqlQuery createIndexes(db);

    if (!createIndexes.exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_users_name  ON users(name)")) {
        qWarning() << "[users] unique index on name failed:" << createIndexes.lastError().text();
        return false;
    }

    if (!createIndexes.exec("CREATE UNIQUE INDEX IF NOT EXISTS idx_users_email ON users(email)")) {
        qWarning() << "[users] unique index on email failed:" << createIndexes.lastError().text();
        return false;
    }

    qDebug() << "[users] schema ready";
    return true;
}

// ---- public API ------------------------------------------------------------

bool DatabaseManager::initUserDatabase()
{
    s_userDb = QSqlDatabase::addDatabase("QSQLITE", "user");
    const QString filePath = dataFilePath("userdb.sqlite");
    s_userDb.setDatabaseName(filePath);

    if (!s_userDb.open()) {
        qWarning() << "Failed to open user DB:" << s_userDb.lastError().text()
                   << "at" << filePath;
        return false;
    }
    qDebug() << "userdb opened at:" << s_userDb.databaseName();

    if (!ensureUsersTableAndIndexes(s_userDb))
        return false;

    return true;
}

bool DatabaseManager::initMediaDatabase()
{
    s_mediaDb = QSqlDatabase::addDatabase("QSQLITE", "media");
    const QString filePath = dataFilePath("mediadb.sqlite");
    s_mediaDb.setDatabaseName(filePath);

    if (!s_mediaDb.open()) {
        qWarning() << "Failed to open media DB:" << s_mediaDb.lastError().text()
                   << "at" << filePath;
        return false;
    }
    qDebug() << "mediadb opened at:" << s_mediaDb.databaseName();

    QSqlQuery createMediaTable(s_mediaDb);
    const char* createSql =
        "CREATE TABLE IF NOT EXISTS media ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  title TEXT NOT NULL"
        ")";
    if (!createMediaTable.exec(createSql)) {
        qWarning() << "Failed to create media table:" << createMediaTable.lastError().text();
        return false;
    }

    qDebug() << "media table ready";
    return true;
}

QSqlDatabase DatabaseManager::connection(const QString& which)
{
    // Keep the string-based API for compatibility.
    // Accepts: "media" -> media DB; anything else -> user DB.
    if (which.compare(QStringLiteral("media"), Qt::CaseInsensitive) == 0)
        return s_mediaDb;
    return s_userDb;
}
