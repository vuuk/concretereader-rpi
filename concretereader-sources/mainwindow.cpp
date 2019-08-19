#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
#include "menuwidget.h"
settings_t MainWindow::user_settings;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)//,
//    graph(new GraphWidget(this)), menu( new MenuWidget(&user_settings, this))
{
    ui->setupUi(this);
  //  menu->hide();
 //   graph->hide();
  //  connect(ui->menu_btn, &QPushButton::clicked, this, &MainWindow::show_menu);
   // connect(ui->graph_btn, &QPushButton::clicked, this, &MainWindow::show_graph);
   // connect(ui->show_widget, &QWidget::)
    //connect(ui->menu_btn, &QPushButton::clicked, this, &MainWindow::show_menu);
  //  connect(ui->chart_btn, &QPushButton::clicked, this, &MainWindow::show_graph);
    //QThread *thread1 = new QThread;
    //thread1->start();
   // ui->graph_tab->moveToThread(thread1);
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
   // ui->show_widget = new GraphWidget(this);
  //  ui->show_widget = graph;
    //ui->show_widget->showFullScreen();
    //ui->show_widget->setFocus();
  //  this->setCentralWidget(ui->show_widget);
 //   ui->menu_btn->hide();
  //  ui->graph_btn->hide();
}

void MainWindow::show_menu()
{
   // ui->show_widget = new MenuWidget(&user_settings, this);
  //  ui->menu_btn->hide();
    //ui->start_btn->hide();
  //  ui->show_widget = menu;
   // ui->show_widget->showFullScreen();
   // ui->show_widget->setFocus();
 //   ui->menu_btn->hide();
   // ui->graph_btn->hide();
   // this->setCentralWidget(ui->show_widget);
}
