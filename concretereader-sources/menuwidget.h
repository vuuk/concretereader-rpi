#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
//#include "mainwindow.h"
class GraphWidget;
namespace Ui {
class MenuWidget;
}

//class settings_t;
class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();
public slots:
    void save();
private slots:
    void update_x_label();
    void update_y_label();
    void update_time_label();
    void update_buff_label();
private:
    Ui::MenuWidget *ui;
};

#endif // MENUWIDGET_H
