#ifndef USER_H
#define USER_H

#include <QString>
#include <QImage>
#include <QDateTime>

//enum class EOnlineStatus {
//    EOS_Online = "Online",
//    EOS_Offline = "Offline"
//};

class User
{
public:
    User(int id, QString email, QString firstName,
         QString lastName, QDateTime registrationDate);

    /* GETTERS */
    int getId() const; // get id
    QString getEmail() const; // get email
    QString getFirstName() const; // get first name
    QString getLastName() const; // get last name
    QDateTime getRegistrationDate() const;
    //QImage getProfilePic() const;
    //EOnlineStatus getOnlineStatus() const;

    /* SETTERS */
    //void setProfilePic(const QImage &image);
    //void setOnlineStatus(EOnlineStatus status);

private:
    /*USER INFO*/
    int id;
    QString email;
    QString firstName;
    QString lastName;
    QDateTime registrationDate;
    //QImage profilePic;
    //EOnlineStatus onlineStatus;
};

#endif // USER_H
