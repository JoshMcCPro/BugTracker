#ifndef USERREGISTRATION_H
#define USERREGISTRATION_H

#include <QString>

class UserRegistration
{
public:
    UserRegistration();
    UserRegistration(const QString& email, const QString& firstName, const QString& lastName, const QString& password, const QString& salt);

    void displayUserInfo();

    QString email;
    QString firstName;
    QString lastName;
    QString password;
    QString salt;

};

#endif // USERREGISTRATION_H
