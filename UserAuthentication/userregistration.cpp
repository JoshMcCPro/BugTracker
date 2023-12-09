#include "userregistration.h"
#include <QDebug>

UserRegistration::UserRegistration()
{

}


UserRegistration::UserRegistration(const QString& email, const QString& firstName, const QString& lastName, const QString& password, const QString& salt)
    : email(email), firstName(firstName), lastName(lastName), password(password), salt(salt) {}

void UserRegistration::displayUserInfo()
{
    qDebug() << "User Registration Info";
    qDebug() << "Email: " << email;
    qDebug() << "First Name: " << firstName;
    qDebug() << "Last Name: " << lastName;
    qDebug() << "Encrypted Password Hex: " << password;
    qDebug() << "salt Hex: " << salt;
}
