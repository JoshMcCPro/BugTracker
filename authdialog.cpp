#include "authdialog.h"
#include "ui_authdialog.h"
#include "QDialog"
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

AuthDialog::AuthDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AuthDialog),
    mDbConnection("serverbugtracker.database.windows.net",
                    "SQL SERVER",
                    "JoshMcCPro",
                    "Jam8122233302!",
                    "DB_BUGTRACKER")
{
    ui->setupUi(this);
}

AuthDialog::~AuthDialog()
{
    delete ui;
}

void AuthDialog::on_pb_login_login_clicked()
{
    QString error;
    if(!mDbConnection.openDatabase(&error)) {
        qDebug() << error;
        return;
    }
    else {
        qDebug() << "Connected to DB";

        // Retrieve user login input

        QString email = ui->le_email_login->text();
        QString password = ui->le_password_login->text();
        QString salt = generateSalt();
        int iterations = 10;
        quint64 keylength = 20;

        // verify user login input

        // check if theres any input first
        if(email.isEmpty() || password.isEmpty()) {qDebug() << "Info Required"; return;}

        // Make email query

        // check if
//        if(!query.exec()) {
//            // Login Failed
//            qDebug() << "Login Failed!";
//        }
//        else {
//            // Login Success
//            qDebug() << "Login Success!";
//            MainWindow *mw = new MainWindow;
//            mw->show();
//            this->close();
//        }

        return;
    }

}

void AuthDialog::loginAuth() {

}

QString AuthDialog::generateSalt()
{
    QRandomGenerator randomGenerator = QRandomGenerator::securelySeeded();
    QByteArray salt;

    for (int i = 0; i < 2; ++i) {
        quint32 randomNum = randomGenerator.global()->generate();

        randomNum = qToLittleEndian(randomNum);

        salt.append(reinterpret_cast<const char*>(&randomNum), sizeof(randomNum));
    }

    salt.resize(8);

    return salt.toHex();
}

QString AuthDialog::encryptPassword(const QString &password, const QString &salt, int iterations, quint64 keylength)
{
    // Convert the password and salt to UTF-8 encoding
    QByteArray passwordData = password.toUtf8();
    QByteArray saltData = QByteArray::fromHex(salt.toUtf8());

    // Perform PBKDF2 key derivation
    QByteArray derivedKey = QPasswordDigestor::deriveKeyPbkdf1(QCryptographicHash::Sha1, passwordData, saltData, iterations, keylength);
    // Convert the derived key to a hex-encoding string
    QString derivedKeyHex = QString(derivedKey.toHex());

    return derivedKeyHex;

}

bool AuthDialog::verifyPassword(const QString &enteredPassword, const QString &storedPassword, const QString &salt, int iterations, quint64 keylength)
{
    QString derivedEnteredPassword = encryptPassword(enteredPassword, salt, iterations, keylength);
    if (derivedEnteredPassword == storedPassword) {
        return true;
    }

    return false;
}

void AuthDialog::on_pb_signuppage_login_clicked()
{
    ui->Pages->setCurrentIndex(signupIndex);


}


void AuthDialog::on_pb_loginpage_signup_clicked()
{
    ui->Pages->setCurrentIndex(loginIndex);
}


void AuthDialog::on_pb_signup_signup_clicked()
{
    // Retrieve user sign up input
    QString email = ui->le_email_signup->text();
    QString firstName = ui->le_firstname_signup->text();
    QString lastName = ui->le_lastname_signup->text();
    QString password = ui->le_password_signup->text();
    QString confirmPassword = ui->le_confirmpwd_signup->text();

    QString error;
    if(!mDbConnection.openDatabase(&error)) {
        qDebug() << error;
        return;
    }

    qDebug() << "Connected to DB";

    // Verify user sign up Input

    // Verify: email

    if (email.isEmpty()) {
        qDebug() << "INVALID: email is empty!";
    }
    else {

        // email format
        QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Z|a-z]{2,}$");

        // email format validator
        QRegularExpressionValidator emailValidator(emailRegex);

        // check if email format is valid
        int pos = 0;
        if (!(emailValidator.validate(email, pos) == QValidator::Acceptable)) {
            qDebug() << "INVALID: email format!";

        }
        else {
            qDebug() << "VALID: email format";

            // Check if email meets the length requirements
            if (!(email.length() <= 100)) {
                qDebug() << "INVALID: email to large!";
            }
            else {
                qDebug() << "VALID: email length";

                // Check if the email is in the database
                QSqlQuery query;
                query.prepare("SELECT email FROM users WHERE email = (:email)");
                query.bindValue(":email", email);

                if (!query.exec()) {
                    qDebug() << "ERROR: couldnt execute query";
                }
                else {
                    if (query.next()) {
                        qDebug() << "Email does exist";
                    }
                    else {
                        qDebug() << "Email doesn't exist";
                        qDebug() << "EMAIL READY";
                    }
                }
            }

        }
    }

    // Verify: first name

    // checks if first name has any input
    if(firstName.isEmpty()) {
        qDebug() << "INVALID: First Name is empty!";
    }
    else {

        // check if first name meets the length requirements
        if(!(firstName.length() <= 50)) {
            qDebug() << "INVALID: First Name is to large!";
        }
        else {
            qDebug() << "VALID: First Name length";
            qDebug() << "FIRST NAME READY";
        }
    }

    // Verify: last name

    // checks if first name has any input
    if(lastName.isEmpty()) {
        qDebug() << "INVALID: Last Name is empty!";
    }
    else {

        // check if first name meets the length requirements
        if(!(lastName.length() <= 50)) {
            qDebug() << "INVALID: Last Name is to large!";
        }
        else {
            qDebug() << "VALID: Last Name length";
            qDebug() << "LAST NAME READY";
        }
    }

    // Verify: password

    if (password.isEmpty()) {
        qDebug() << "INVALID: Password is empty!";
    }
    else {

        // password format
        QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)[A-Za-z0-9]+$");
        // password format validator
        QRegularExpressionValidator passwordValidator(passwordRegex);

        int pos = 0;
        if (!(passwordValidator.validate(password, pos) == QValidator::Acceptable)) {
            qDebug() << "INVALID: password format";
        }
        else {

            if (!(password.length() >= 8)) {
                qDebug() << "INVALID: Password is to small!";
            }
            else {
                qDebug() << "VALID: Password length";

                qDebug() << "PASSWORD READY";

            }
        }
    }

    // Verify: confirm password

    if (confirmPassword != password) {
        qDebug() << "INVALID: confirm password and password dont match";
    }
    else {
        qDebug() << "CONFIRM PASSWORD READY";
    }

    // Sign up user

    // store user info
    // send email verification code
    // go to code verification page
    // Once entered send user info to SQL database
    // login
}

QString AuthDialog::generateVerificationCode(int codeLength)
{
    QRandomGenerator randCodeGen = QRandomGenerator::securelySeeded();
    QVector<int> verificationCode(codeLength, 0);
    QString Code = "";

    for (int i = 0; i < codeLength; i++) {
        quint32 randNum = randCodeGen.bounded(0,9);
        verificationCode[i] = randNum;
        Code += QString::number(randNum);
    }

    return Code;
}

void AuthDialog::sendEmailVerificationCode(QString recieverEmail)
{
    QString host = "smtp.gmail.com";
    int port = 465;
    QString username = "JoshMcCPro@gmail.com";
    QString appPassword = "hyif qkrw bkna dltm";

    int verificationCodeLength = 6;
    QString code = generateVerificationCode(verificationCodeLength);

    qDebug() << code;
    return;

    // Construct the email message
    QString subject = "Email Verification Code";
    QString body = "Your verification code is: " + code;
    QString from = "JoshMcCPro@gmail.com";
    QString to = recieverEmail;

    // Connect to the SMTP server
    QSslSocket socket;
    socket.setProtocol(QSsl::TlsV1_3);

    socket.connectToHostEncrypted(host, port);
    if (!socket.waitForConnected()) {
        qDebug() << "Error connecting to the SMTP server: " << socket.errorString();
        return;
    }

    // Read the welcome message from the server
    if (!socket.waitForReadyRead()) {
        qDebug() << "Error reading welcome message: " << socket.errorString();
        return;
    }
    qDebug() << "Server says: " << socket.readAll();

    // Send the HELO command
    socket.write("HELO localhost\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the login command
    socket.write("AUTH LOGIN\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the username (Gmail address)
    socket.write((username.toUtf8() + "\r\n").toBase64() + "\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send app password
    socket.write((appPassword.toUtf8() + "\r\n").toBase64() + "\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the MAIL FROM command
    socket.write(("MAIL FROM: <" + from + ">\r\n").toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says: " << socket.readAll();

    // Send the RCPT TO command
    socket.write(("RCPT TO: <" + to + ">\r\n").toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Send the DATA command
    socket.write("DATA\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Construct and send the email message
    QString message =
        "From: " + from + "\r\n"
                          "To: " + to + "\r\n"
               "Subject: " + subject + "\r\n"
                    "\r\n" + body + "\r\n"
                 ".\r\n";  // End of message marker

    socket.write(message.toUtf8());
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Send the QUIT command
    socket.write("QUIT\r\n");
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qDebug() << "Server says:" << socket.readAll();

    // Close the socket
    socket.close();
}

void AuthDialog::on_le_email_signup_editingFinished()
{
    qDebug() << "Email finished";
}


void AuthDialog::on_le_firstname_signup_editingFinished()
{
    QString firstName = ui->le_firstname_signup->text();

    // Verify: first name

    // checks if first name has any input
    if(firstName.isEmpty()) {
        qDebug() << "INVALID: First Name is empty!";
    }
    else {

        // check if first name meets the length requirements
        if(!(firstName.length() <= 50)) {
            qDebug() << "INVALID: First Name is to large!";
        }
        else {
            qDebug() << "VALID: First Name length";
            qDebug() << "FIRST NAME READY";
        }
    }
}


void AuthDialog::on_le_lastname_signup_editingFinished()
{
    QString lastName = ui->le_lastname_signup->text();

    // Verify: last name

    // checks if first name has any input
    if(lastName.isEmpty()) {
        qDebug() << "INVALID: Last Name is empty!";
    }
    else {

        // check if first name meets the length requirements
        if(!(lastName.length() <= 50)) {
            qDebug() << "INVALID: Last Name is to large!";
        }
        else {
            qDebug() << "VALID: Last Name length";
            qDebug() << "LAST NAME READY";
        }
    }
}


void AuthDialog::on_le_password_signup_editingFinished()
{
    QString password = ui->le_password_signup->text();

    // Verify: password

    if (password.isEmpty()) {
        qDebug() << "INVALID: Password is empty!";
    }
    else {

        // password format
        QRegularExpression passwordRegex("^(?=.*[A-Z])(?=.*[a-z])(?=.*\\d)[A-Za-z0-9]+$");
        // password format validator
        QRegularExpressionValidator passwordValidator(passwordRegex);

        int pos = 0;
        if (!(passwordValidator.validate(password, pos) == QValidator::Acceptable)) {
            qDebug() << "INVALID: password format";
        }
        else {

            if (!(password.length() >= 8)) {
                qDebug() << "INVALID: Password is to small!";
            }
            else {
                qDebug() << "VALID: Password length";

                qDebug() << "PASSWORD READY";

            }
        }
    }
}


void AuthDialog::on_le_confirmpwd_signup_editingFinished()
{
    QString confirmPassword = ui->le_confirmpwd_signup->text();
    QString password = ui->le_password_signup->text();

    // Verify: confirm password

    if (confirmPassword != password) {
        qDebug() << "INVALID: confirm password and password dont match";
    }
    else {
        qDebug() << "CONFIRM PASSWORD READY";
    }
}

