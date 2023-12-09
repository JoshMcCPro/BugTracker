#ifndef PROJECTINVITEWIDGET_H
#define PROJECTINVITEWIDGET_H

#include <QWidget>

namespace Ui {
class ProjectInviteWidget;
}

class ProjectInviteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectInviteWidget(QWidget *parent = nullptr);
    ~ProjectInviteWidget();

private:
    Ui::ProjectInviteWidget *ui;
};

#endif // PROJECTINVITEWIDGET_H
