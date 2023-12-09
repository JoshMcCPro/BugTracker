#ifndef PROJECTDETAILSWIDGET_H
#define PROJECTDETAILSWIDGET_H

#include "project.h"
#include <QWidget>

namespace Ui {
class ProjectDetailsWidget;
}

class ProjectDetailsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectDetailsWidget(QWidget *parent = nullptr);
    ~ProjectDetailsWidget();
    
    void setProjectDetails(const Project &project);

signals:
    void openIssues();

private slots:
    void on_pb_Dashboard_clicked();

    void on_pb_Issues_clicked();

    void on_pb_Messages_clicked();

private:
    Ui::ProjectDetailsWidget *ui;
};

#endif // PROJECTDETAILSWIDGET_H
