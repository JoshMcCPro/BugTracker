#ifndef USERAUTHDIALOG_H
#define USERAUTHDIALOG_H

#include <QDialog>

class QStackedWidget;
class LoginWidget;
class SignUpWidget;
class VerifyCodeWidget;
class UserRegistration;

namespace Ui {
class UserAuthDialog;
}

class UserAuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserAuthDialog(QWidget *parent = nullptr);
    ~UserAuthDialog();

    void setPage(int pageIndex);

    void getLoginPage();
    void getSignUpPage();
    void getVerifyCodePage(const UserRegistration& user);


public slots:
    void handleLoginSuccessful(QString email);

signals:
    void loginSuccessful(QString email);

private:
    Ui::UserAuthDialog *ui;

    QStackedWidget *stackedWidget;
    LoginWidget *loginWidget;
    SignUpWidget *signUpWidget;
    VerifyCodeWidget *verifyCodeWidget;

    int loginIndex = 0;
    int signUpIndex = 1;
    int verifyCodeIndex = 2;
};

#endif // USERAUTHDIALOG_H
