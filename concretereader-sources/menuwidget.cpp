#include "menuwidget.h"
#include "ui_menuwidget.h"
#include "graphwidget.h"
MenuWidget::MenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuWidget),
    graph(nullptr)
{
    ui->setupUi(this);
}

MenuWidget::~MenuWidget()
{
    delete ui;
}

void MenuWidget::show_graph()
{
    GraphWidget* new_graph = new GraphWidget(this);
    graph = new_graph;
    ui->view_layout->addWidget(new_graph);
}
