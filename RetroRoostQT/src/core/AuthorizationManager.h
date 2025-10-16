//
// Created by Jarrid on 9/25/2025.
//

#ifndef RETROROOSTQT_AUTHORIZATIONMANAGER_H
#define RETROROOSTQT_AUTHORIZATIONMANAGER_H

#include <QObject>
#include <QString>

class AuthorizationManager : public QObject {
    Q_OBJECT
public:
    using QObject::QObject;

    Q_INVOKABLE static bool login(const QString& email, const QString& password);
};

#endif //RETROROOSTQT_AUTHORIZATIONMANAGER_H