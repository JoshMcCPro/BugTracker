#include "mainwindow.h"
#include "authdialog.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");
    AuthDialog ad;
    ad.show();

    return a.exec();
}
