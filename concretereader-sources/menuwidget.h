#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>
class GraphWidget;
namespace Ui {
class MenuWidget;
}

class MenuWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MenuWidget(QWidget *parent = nullptr);
    ~MenuWidget();

public slots:
    void show_graph();

private:
    Ui::MenuWidget *ui;
    GraphWidget* graph;
};

#endif // MENUWIDGET_H
