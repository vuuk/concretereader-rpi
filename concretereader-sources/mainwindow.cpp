#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

  //  connect(ui->chart_btn, &QPushButton::clicked, this, &MainWindow::show_graph);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_graph()
{
  //  ui->view_widget = new GraphWidget(this);
   // ui->view_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  //  ui->view_widget->show();
}
