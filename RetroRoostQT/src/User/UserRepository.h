//
// Created by Jarrid on 9/22/2025.
//

#ifndef RETROROOSTQT_USERREPOSITORY_H
#define RETROROOSTQT_USERREPOSITORY_H

#include <QList>
#include "User.h"

// UserRepository.h
class UserRepository {
public:
    static bool createUser(const QString& name, const QString& email, const QString& password);
    static QList<User> getAllUsers();
};


#endif //RETROROOSTQT_USERREPOSITORY_H