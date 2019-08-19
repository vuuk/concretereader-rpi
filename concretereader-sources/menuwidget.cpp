#include "menuwidget.h"
#include "ui_menuwidget.h"
#include "graphwidget.h"
#include "mainwindow.h"

MenuWidget::MenuWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuWidget)//,
    //m_settings(settings)
   // graph(nullptr)
{
    ui->setupUi(this);
    //connect(ui->close_btn, &QPushButton::clicked, this, &MenuWidget::close);
    connect(ui->x_range_slider, &QSlider::valueChanged, this, &MenuWidget::update_x_label);
    connect(ui->y_range_slider, &QSlider::valueChanged, this, &MenuWidget::update_y_label);
    connect(ui->time_update_slider, &QSlider::valueChanged, this, &MenuWidget::update_time_label);
    connect(ui->buff_size_slider, &QSlider::valueChanged, this, &MenuWidget::update_buff_label);
    connect(ui->save_btn, &QPushButton::clicked, this, &MenuWidget::save);
    ui->y_current_label->setText(QString("%1").arg(MainWindow::user_settings.getYmax(), 4));
    ui->x_current_label->setText(QString("%1").arg(MainWindow::user_settings.getXmax(), 3));
    ui->time_current_label->setText(QString("%1").arg(MainWindow::user_settings.getInterval(), 3));
    ui->buff_current_label->setText(QString("%1").arg(MainWindow::user_settings.getBuffer(), 4));
  //  connect(ui->stop_btn, &QPushButton::clicked, this, &GraphWidget::stop);
}

MenuWidget::~MenuWidget()
{
    delete ui;
}


void MenuWidget::update_x_label()
{
    ui->x_current_label->setText(QString("%1").arg(ui->x_range_slider->value(), 3));
}

void MenuWidget::update_y_label()
{
   // ui->y_current_label->setText(ui->y_range_slider->value());
     ui->y_current_label->setText(QString("%1").arg(ui->y_range_slider->value(), 4));
}

void MenuWidget::update_time_label()
{
    //ui->time_current_label->setText(ui->time_update_slider->value());
    ui->time_current_label->setText(QString("%1").arg(ui->time_update_slider->value(), 3));
}

void MenuWidget::update_buff_label()
{
    //ui->time_current_label->setText(ui->time_update_slider->value());
    ui->buff_current_label->setText(QString("%1").arg(ui->buff_size_slider->value(), 4));
}
void MenuWidget::save()
{
    MainWindow::user_settings.setInterval(ui->time_update_slider->value());
    MainWindow::user_settings.setXmax(ui->x_range_slider->value());
    MainWindow::user_settings.setYmax(ui->y_range_slider->value());
    MainWindow::user_settings.setBuffer(ui->buff_size_slider->value());
}
