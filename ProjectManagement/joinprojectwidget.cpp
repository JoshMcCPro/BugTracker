#include "joinprojectwidget.h"
#include "ui_joinprojectwidget.h"

JoinProjectWidget::JoinProjectWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JoinProjectWidget)
{
    ui->setupUi(this);
}

JoinProjectWidget::~JoinProjectWidget()
{
    delete ui;
}
