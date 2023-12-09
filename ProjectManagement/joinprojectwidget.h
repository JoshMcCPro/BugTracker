#ifndef JOINPROJECTWIDGET_H
#define JOINPROJECTWIDGET_H

#include <QWidget>

namespace Ui {
class JoinProjectWidget;
}

class JoinProjectWidget : public QWidget
{
    Q_OBJECT

public:
    explicit JoinProjectWidget(QWidget *parent = nullptr);
    ~JoinProjectWidget();

private:
    Ui::JoinProjectWidget *ui;
};

#endif // JOINPROJECTWIDGET_H
