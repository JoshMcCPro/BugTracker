#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

// side detail widgets
#include "userdetailswidget.h"
#include "projectdetailswidget.h"

// user page widgets
#include "ProjectManagement/projectmanagerwidget.h"
#include "ProjectManagement/IssueManagement/issuemanager.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // import user with main window to ensure its valid
    MainWindow(User *user, QWidget *parent = nullptr);
    ~MainWindow();

    /*
    *   LEFT SIDE BAR NAVIGATION WIDGET
    */

    // opens the users details widget
    void openUserDetails();
    // opens the project's details widget
    void openProjectDetails(const Project& project);

    // open pages from side bar
    void openPage(const QString& pageName);

public slots:
    /*USER DETAILS BUTTONS*/
    // Opens users project page
    void handleOpenProject();

    void handleOpenProjectDetails(const Project& project);

    /*PROJECT DETAILS BUTTONS*/
    // opens project issues page
    void handleOpenIssues();

private:
    Ui::MainWindow *ui;

    /*LEFT SIDE BAR*/
    // detail widgets
    QStackedWidget *stackedDetailsWidget;
    UserDetailsWidget *userDetailsWidget;
    ProjectDetailsWidget *projectDetailsWidget;

    // left side bar pages
    int userNavIndex = 0;
    int projectNavIndex = 1;

    /*DETAILS PAGE WIDGET*/
    QStackedWidget *stackedDetailPageWidget;

    /*USER PAGE WIDGETS*/
    ProjectManagerWidget *projectManager;

    /*PROJECT PAGE WIDGETS*/

    IssueManager *issueManager;

    // User info
    User *user;

};
#endif // MAINWINDOW_H
