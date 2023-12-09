#include "projectinvitewidget.h"
#include "ui_projectinvitewidget.h"

ProjectInviteWidget::ProjectInviteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectInviteWidget)
{
    ui->setupUi(this);
}

ProjectInviteWidget::~ProjectInviteWidget()
{
    delete ui;
}
