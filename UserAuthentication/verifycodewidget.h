#ifndef VERIFYCODEWIDGET_H
#define VERIFYCODEWIDGET_H

#include <QWidget>
#include "userregistration.h"
#include "databaseconnection.h"
#include "userauthdialog.h"

namespace Ui {
class VerifyCodeWidget;
}

class VerifyCodeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VerifyCodeWidget(QWidget *parent = nullptr);
    ~VerifyCodeWidget();

    void setUserInformation(const UserRegistration& user);
    void displayUserInfo();

    // email verification
    QString generateVerificationCode();
    void sendEmailVerificationCode();

    void confirmUserAccount();

    // pages
    void goToLoginPage();

    QWidget *mainParentWidget;
    UserAuthDialog *userAuthDialog;

private slots:

    void on_pb_submitCode_clicked();

private:
    Ui::VerifyCodeWidget *ui;

    UserRegistration user;

    // SQL DB
    QString mDbConnectionName = "verificationCodeConnection";
    DatabaseConnection mDbConnection;

    // Email
    QString host = "smtp.gmail.com";
    int port = 465;
    QString senderEmail = "JoshMcCPrO@gmail.com";
    QString appPassword = "hyif qkrw bkna dltm";
    QString emailCode;
    int verificationCodeLength = 6;
};

#endif // VERIFYCODEWIDGET_H
