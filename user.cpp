#include "user.h"

#include <QDateTime>

User::User(int id, QString email, QString firstName, QString lastName, QDateTime registrationDate)
{
    this->id = id;
    this->email = email;
    this->firstName = firstName;
    this->lastName = lastName;
    this->registrationDate = registrationDate;
}

/* GETTERS */

int User::getId() const
{
    return this->id;
}

QString User::getEmail() const
{
    return this->email;
}

QString User::getFirstName() const
{
    return this->firstName;
}

QString User::getLastName() const
{
    return this->lastName;
}

QDateTime User::getRegistrationDate() const
{
    return this->registrationDate;
}
