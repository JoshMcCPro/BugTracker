#include "loginwidget.h"
#include "ui_loginwidget.h"
#include "userauthdialog.h"
#include "databaseconnection.h"
#include "mainwindow.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QtEndian>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include <QCryptographicHash>
#include <QByteArray>
#include <QPasswordDigestor>

#include <QTcpSocket>
#include <QSslSocket>

LoginWidget::LoginWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWidget),
    mDbConnection("serverbugtracker.database.windows.net",
                  "SQL SERVER",
                  "JoshMcCPro",
                  "Jam8122233302!",
                  "DB_BUGTRACKER",
                  mDbConnectionName)
{
    ui->setupUi(this);

}

LoginWidget::~LoginWidget()
{
    delete ui;
    delete mainParentWidget;
    delete userAuthDialog;
}

void LoginWidget::on_pb_login_clicked()
{

    QString enteredEmail = ui->le_email->text();
    QString enteredPassword = ui->le_password->text();

    // check if email is valid
    if (!isEmailValid(enteredEmail)) {
        return;
    }

    // check if password is valid
    if (!isPasswordValid(enteredPassword)) {
        return;
    }

    // connect to db
    QString error;
    if(!mDbConnection.openDatabase(&error)) {
        qDebug() << "Falied to open database: " << error;
        return;
    }

    qDebug() << "Connected to DB";

    // check if email exists in db
    // if it doesnt exist stop
    if(!doesEmailExist(enteredEmail)) {
        qDebug() << "Email doesnt exist";
        mDbConnection.closeDatabase();
        return;
    }

    // if email exist check if passwords match
    const QString storedSalt = getSaltFromDatabase(enteredEmail);
    const QString storedPassword = getEncryptedPasswordFromDatabase(enteredEmail);
    const int iterations = 12;
    const int keyLength = 20;

    if (bDoesPasswordExist(enteredPassword, storedPassword, storedSalt, iterations, keyLength))
    {
        // if the password exist login
        qDebug() << "Logging In ...";
        emit loginSuccessful(enteredEmail);
    }
    else {
        qDebug() << "Invalid password";
    }

    mDbConnection.closeDatabase();

}


void LoginWidget::on_pb_signup_clicked()
{

    mainParentWidget = this->parentWidget()->parentWidget();

    // Use qobject_cast for type-safe casting
    if (userAuthDialog = qobject_cast<UserAuthDialog*>(mainParentWidget)) {
        qDebug() << "Sign Up Page!";
        userAuthDialog->getSignUpPage();
    } else {
        // Handle the case where the cast is not successful
        qDebug() << "Error: Parent is not of type UserAuthDialog";
        qDebug() << "Parent: " << mainParentWidget->metaObject()->className();
    }

}


void LoginWidget::on_le_email_editingFinished()
{
    QString enteredEmail = ui->le_email->text();
    // check if email is valid
    if (!isEmailValid(enteredEmail)) {
        return;
    }

}


void LoginWidget::on_le_password_editingFinished()
{
    QString enteredPassword = ui->le_password->text();
    // check if password is valid
    if (!isPasswordValid(enteredPassword)) {
        return;
    }
}

bool LoginWidget::isEmailValid(QString enteredEmail)
{
    QString email = enteredEmail;

    if (email.isEmpty()) {
        qDebug() << "INVALID: email is empty!";
        return false;
    }

    // check if email format is valid
    if (!isFormatValid(emailFormat, email)) {
        qDebug() << "INVALID: email format error";
        return false;
    }

    // Check if email meets the length requirements
    if (!(email.length() <= 100)) {
        qDebug() << "INVALID: email to large!";
        return false;
    }

    return true;
}

bool LoginWidget::isPasswordValid(QString enteredPassword)
{
    QString password = enteredPassword;

    if (password.isEmpty()) {
        qDebug() << "INVALID: password is empty!";
        return false;
    }

    // check if password format is valid
    if(!isFormatValid(passwordFormat, password)) {
        qDebug() << "INVALID: password needs at least 1 captial, 1 lower, and 1 number";
        return false;
    }

    // check if password meets the length requirments
    if (!(password.length() >= 8)) {
        qDebug() << "INVALID: password needs to be at least 8 characters!";
        return false;
    }

    return true;
}

bool LoginWidget::isFormatValid(QString format, QString userCredential)
{
    // make format
    QRegularExpression regEx(format);

    // email format validator
    QRegularExpressionValidator regExValidator(regEx);

    // check if email format is valid
    int pos = 0;
    if (!(regExValidator.validate(userCredential, pos) == QValidator::Acceptable)) {
        return false;
    }

    return true;
}

bool LoginWidget::doesEmailExist(QString enteredEmail)
{
    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    if(!query.prepare("SELECT COUNT(*) FROM users WHERE email = :email")) {
        qDebug() << "Error preparing query: " << query.lastError().text();
        return false;
    }
    query.bindValue(":email", enteredEmail);

    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            qDebug() << "email count: " << count;
            return true;
        }
    }

    return false;
}


QString LoginWidget::getSaltFromDatabase(QString email)
{
    qDebug() << "Getting salt ...";
    const int expectedSaltLength = 8;
    QString saltHex;

    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));

    if (!query.prepare("SELECT salt FROM passwords WHERE user_id = (SELECT user_id FROM users WHERE email = :email)")) {
        qDebug() << "Error preparing query: " << query.lastError().text();
    }
    query.bindValue(":email", email);

    if (!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        return QString(); // Return an empty string
    }

    if(query.next()) {
        saltHex = query.value("salt").toString();

        // Check the length of the retrieved salt
        if (saltHex.length() != expectedSaltLength * 2) {
            qDebug() << "Warning: Retrieved salt does not have the expected length.";
            return QString();
        }
    }

    qDebug() << "Salt Hex: " << saltHex;
    return saltHex;
}

QString LoginWidget::getEncryptedPasswordFromDatabase(QString email)
{
    qDebug() << "Getting encrypted password ...";
    QString encryptedPwd;
    QSqlQuery query(QSqlDatabase::database(mDbConnectionName));;

    if (!query.prepare("SELECT password_hash FROM passwords WHERE user_id = (SELECT user_id FROM users WHERE email = :email)")) {
        qDebug() << "Error preparing query: " << query.lastError().text();
        return QString();
    }
    query.bindValue(":email", email);

    if(!query.exec()) {
        qDebug() << "Error executing query: " << query.lastError().text();
        return encryptedPwd;
    }

    if(query.next()) {
        encryptedPwd = query.value("password_hash").toString();
    }

    qDebug() << "Encrypted Password: " << encryptedPwd;
    return encryptedPwd;
}

// encrypt password
QString LoginWidget::encryptPassword(const QString &password, const QString &salt, int iterations, quint64 keyLength)
{
    // Convert the password and salt to UTF-8 encoding
    QByteArray passwordData = password.toUtf8();
    QByteArray saltData = QByteArray::fromHex(salt.toUtf8());

    qDebug() << "password: " << password;
    qDebug() << "salt: " << saltData;
    qDebug() << "salt hex: " << salt;

    // Perform PBKDF2 key derivation
    QByteArray derivedKey = QPasswordDigestor::deriveKeyPbkdf1(QCryptographicHash::Sha1, passwordData, saltData, iterations, keyLength);
    // Convert the derived key to a hex-encoding string
    QString derivedKeyHex = QString(derivedKey.toHex());

    return derivedKeyHex;
}

// check if password exists
bool LoginWidget::bDoesPasswordExist(const QString &enteredPassword, const QString &storedPassword, const QString &salt, int iterations, quint64 keylength)
{
    QString derivedEnteredPassword = encryptPassword(enteredPassword, salt, iterations, keylength);
    return derivedEnteredPassword == storedPassword;
}



