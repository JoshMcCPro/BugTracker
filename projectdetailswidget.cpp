#include "projectdetailswidget.h"
#include "ui_projectdetailswidget.h"

ProjectDetailsWidget::ProjectDetailsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectDetailsWidget)
{
    ui->setupUi(this);
}

ProjectDetailsWidget::~ProjectDetailsWidget()
{
    delete ui;
}

void ProjectDetailsWidget::setProjectDetails(const Project &project)
{
    ui->lb_username->setText(project.getTitle());
}

// emits signal to open dashboard page
void ProjectDetailsWidget::on_pb_Dashboard_clicked()
{

}

// emits signal to open issues page
void ProjectDetailsWidget::on_pb_Issues_clicked()
{
    // insert issues page into main window
    emit openIssues();
}

// emits signal to open messages page
void ProjectDetailsWidget::on_pb_Messages_clicked()
{

}

