//
// Created by Jarrid on 9/25/2025.
//

#include "AuthorizationManager.h"
#include "../repositories/UserRepository.h"
#include <QDebug>


bool AuthorizationManager::login(const QString& email, const QString& password)
{
    UserRepository repo;

    auto user = repo.findUserByEmailAndPassword(email, password);

    if (user.has_value()) {
        qDebug().noquote()
            << "[auth] Login successful for:"
            << user->GetEmail();
        return true;
    }

    qWarning().noquote()
        << "[auth] Login failed for email:" << email;
    return false;
}
