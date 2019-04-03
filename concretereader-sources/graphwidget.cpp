#include "graphwidget.h"
#include "ui_graphwidget.h"
#include "serialcomhandler.h"
#include <iostream>
#include <QDebug>
#include <QLineSeries>
#include <QChart>
#include <QFont>

const std::string g_PORT = "/dev/ttyUSB0";
const std::string g_PATTERN = "[\\r]{1}([\\d]+)[\\n]{1}";


GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWidget),
    reading_timer(new QTimer(this)),
    handler(SerialcomHandler<concrete_reading>::instance()),
    current_series(new QLineSeries),
    current_chart(new QChart),
    prev_max(X_RANGE)
{
    QFont stnd;
    stnd.setPointSize(7);
    //stnd.setPixelSize(1);
    //ui->chart->setFont(stnd);
   // current_chart->setFont(stnd);
    ui->setupUi(this);
   // current_series->s
    handler.setPortName(g_PORT);
    handler.setRegexPattern(g_PATTERN);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, prev_max);
    axisX->setLabelsFont(stnd);
   // axisX->setLabelFormat("%ms");
   // axisX->setTitleText("ms");
   // axisX->setTitleVisible(false);
    axisX->setLabelsVisible();

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, Y_RANGE);
    axisY->setLabelsVisible();
    axisY->setLabelsFont(stnd);
   // axisY->setLabelFormat("%A");
    //axisY->setTitleText("A");
    current_chart->setContentsMargins(0,0,0,0);
    current_chart->addSeries(current_series);
    current_chart->setAxisX(axisX, current_series);
    current_chart->setAxisY(axisY, current_series);
    current_chart->legend()->hide();
 //   current_chart->createDefaultAxes();
    //current_chart->setTitle("Example");
   // current_chart->setX(100);
    //current_chart->setY(4000);
    ui->chart->setChart(current_chart);
    connect(ui->start_btn, &QPushButton::clicked, this, &GraphWidget::start);
    connect(ui->stop_btn, &QPushButton::clicked, this, &GraphWidget::stop);

    connect(reading_timer, &QTimer::timeout, this, &GraphWidget::get_readings);
}

GraphWidget::~GraphWidget()
{
    delete ui;
}

void GraphWidget::start()
{
    current_series->clear();
    reading_timer->start(1);
    //handler.start_sensors();
    handler.start_concrete();

    concrete_reading::set_start_time();
}

void GraphWidget::stop()
{
    reading_timer->stop();
    handler.stop_concrete();
    std::cerr << current_results.size() << std::endl;
}

void GraphWidget::get_readings()
{
    while (handler.available_reads()) {
        concrete_reading&& tmp = handler.read_available();
      //  std::cerr << tmp << ".";
        //current_results.push_back(tmp);
        current_series->append(tmp.p);
        if (tmp.p.x() >= prev_max)
        {
            current_chart->axisX()->setRange(prev_max - 1000, prev_max + 21000);
            prev_max = prev_max + 20000;
        }
       // ui->chart->update();
      //  std::cerr << tmp << std::endl;
    }
}
