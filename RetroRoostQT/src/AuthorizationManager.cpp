//
// Created by Jarrid on 9/25/2025.
//

#include "AuthorizationManager.h"
#include "User/UserRepository.h"

bool AuthorizationManager::login(const QString& email, const QString& password) {
    auto user = UserRepository::findUserByEmailAndPassword(email, password);
    return user.has_value();
}
