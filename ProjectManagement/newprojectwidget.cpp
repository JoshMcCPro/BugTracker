#include "newprojectwidget.h"
#include "ui_newprojectwidget.h"
#include "ProjectManagement/projectmanagerwidget.h"


NewProjectWidget::NewProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewProjectWidget)
{
    ui->setupUi(this);

}

NewProjectWidget::~NewProjectWidget()
{
    delete ui;
}

void NewProjectWidget::on_pb_CreateProject_clicked()
{

    emit openCreateProject();
}


void NewProjectWidget::on_pb_JoinProject_clicked()
{
    emit openJoinProject();
}

