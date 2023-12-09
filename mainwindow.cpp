#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userdetailswidget.h"
#include "projectdetailswidget.h"
#include "ProjectManagement/projectmanagerwidget.h"
#include "ProjectManagement/IssueManagement/issuemanager.h"

MainWindow::MainWindow(User *user, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , user(user)
{
    ui->setupUi(this);

    // initializing details widgets
    stackedDetailsWidget = ui->UserNavigationStackedWidget;
    userDetailsWidget = new UserDetailsWidget(this->user, this);
    projectDetailsWidget = new ProjectDetailsWidget(this);

    // add user & project detail widgets to stack
    stackedDetailsWidget->addWidget(userDetailsWidget);
    stackedDetailsWidget->addWidget(projectDetailsWidget);

    stackedDetailPageWidget = ui->dataStackedWidget;

    // User details button connections
    connect(userDetailsWidget, &UserDetailsWidget::openProjects, this, &MainWindow::handleOpenProject);

    // Project details button connections
    connect(projectDetailsWidget, &ProjectDetailsWidget::openIssues, this, &MainWindow::handleOpenIssues);

    // open user details
    openUserDetails();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete stackedDetailsWidget;
    delete stackedDetailPageWidget;
}
// open user nav
void MainWindow::openUserDetails()
{
    stackedDetailsWidget->setCurrentIndex(userNavIndex);
}
// open project nav
void MainWindow::openProjectDetails(const Project& project)
{
    projectDetailsWidget->setProjectDetails(project);
    stackedDetailsWidget->setCurrentIndex(projectNavIndex);
}

void MainWindow::openPage(const QString &pageName)
{
    QWidget* newWidget = nullptr;

    // Check if the widget with the specificed pageName is already added
    for (int i = 0; i < stackedDetailPageWidget->count(); ++i) {
        if (stackedDetailPageWidget->widget(i)->objectName() == pageName) {
            newWidget = stackedDetailPageWidget->widget(i);
            break;
        }
    }

    // If not added, create and add it to the stack
    if (!newWidget) {
        if (pageName == "ProjectManagerPage") {
            User pmUser = *user;
            projectManager = new ProjectManagerWidget(pmUser, this);
            // handles project manager action button
                connect(projectManager, &ProjectManagerWidget::onProjectButtonClicked, this, &MainWindow::handleOpenProjectDetails);
            newWidget = projectManager;
        }
        else if (pageName == "IssueManagerPage") {
            issueManager = new IssueManager(this);
            newWidget = issueManager;
        }

        newWidget->setObjectName(pageName);
        stackedDetailPageWidget->addWidget(newWidget);
    }

    stackedDetailPageWidget->setCurrentWidget(newWidget);
}

void MainWindow::handleOpenProject()
{
    openPage("ProjectManagerPage");
}

void MainWindow::handleOpenProjectDetails(const Project& project)
{
    openProjectDetails(project);
    openPage("DefaultPage");
}

void MainWindow::handleOpenIssues()
{
    openPage("IssueManagerPage");
}

