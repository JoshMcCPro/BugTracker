#ifndef COMMUNITYPROJECTSWIDGET_H
#define COMMUNITYPROJECTSWIDGET_H

#include <QWidget>

namespace Ui {
class CommunityProjectsWidget;
}

class CommunityProjectsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CommunityProjectsWidget(QWidget *parent = nullptr);
    ~CommunityProjectsWidget();

private:
    Ui::CommunityProjectsWidget *ui;
};

#endif // COMMUNITYPROJECTSWIDGET_H
