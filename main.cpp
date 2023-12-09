#include "mainwindow.h"
#include "UserAuthentication\userauthdialog.h"
#include "UserAuthentication\loginwidget.h"
#include "applicationmanager.h"

#include <QApplication>
#include <QObject>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setStyle("Fusion");

    UserAuthDialog userAuthDialog;
    ApplicationManager appManager;

    QObject::connect(&userAuthDialog, &UserAuthDialog::loginSuccessful, &appManager, &ApplicationManager::handleLoginSuccess);

    userAuthDialog.show();

    return app.exec();
}
