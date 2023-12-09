QT += core gui
QT += sql
QT += core
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ProjectManagement/IssueManagement/issuemanager.cpp \
    ProjectManagement/communityprojectswidget.cpp \
    ProjectManagement/createprojectwidget.cpp \
    ProjectManagement/joinprojectwidget.cpp \
    ProjectManagement/newprojectwidget.cpp \
    ProjectManagement/projectbutton.cpp \
    ProjectManagement/projectinvitewidget.cpp \
    ProjectManagement/projectmanagerwidget.cpp \
    UserAuthentication/loginwidget.cpp \
    UserAuthentication/signupwidget.cpp \
    UserAuthentication/userauthdialog.cpp \
    UserAuthentication/userregistration.cpp \
    UserAuthentication/verifycodewidget.cpp \
    applicationmanager.cpp \
    databaseconnection.cpp \
    main.cpp \
    mainwindow.cpp \
    project.cpp \
    projectdetailswidget.cpp \
    test/authdialog.cpp \
    user.cpp \
    userdetailswidget.cpp

HEADERS += \
    ProjectManagement/IssueManagement/issuemanager.h \
    ProjectManagement/communityprojectswidget.h \
    ProjectManagement/createprojectwidget.h \
    ProjectManagement/joinprojectwidget.h \
    ProjectManagement/newprojectwidget.h \
    ProjectManagement/projectbutton.h \
    ProjectManagement/projectinvitewidget.h \
    ProjectManagement/projectmanagerwidget.h \
    UserAuthentication/loginwidget.h \
    UserAuthentication/signupwidget.h \
    UserAuthentication/userauthdialog.h \
    UserAuthentication/userregistration.h \
    UserAuthentication/verifycodewidget.h \
    applicationmanager.h \
    authdialog.h \
    databaseconnection.h \
    mainwindow.h \
    project.h \
    projectdetailswidget.h \
    user.h \
    userdetailswidget.h

FORMS += \
    ProjectManagement/IssueManagement/issuemanager.ui \
    ProjectManagement/communityprojectswidget.ui \
    ProjectManagement/createprojectwidget.ui \
    ProjectManagement/joinprojectwidget.ui \
    ProjectManagement/newprojectwidget.ui \
    ProjectManagement/projectinvitewidget.ui \
    ProjectManagement/projectmanagerwidget.ui \
    UserAuthentication/loginwidget.ui \
    UserAuthentication/signupwidget.ui \
    UserAuthentication/userauthdialog.ui \
    UserAuthentication/verifycodewidget.ui \
    authdialog.ui \
    mainwindow.ui \
    projectdetailswidget.ui \
    userdetailswidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ProjectManagement/Resource.qrc \
    Resource.qrc
