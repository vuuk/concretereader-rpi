#include "graphwidget.h"
#include "ui_graphwidget.h"
#include "serialcomhandler.h"
#include <iostream>
#include <QDebug>
#include <QLineSeries>
#include <QChart>
#include <QFont>
#include <fstream>
#include <QDateTime>
#include <thread>
#include "readingsmodel.h"
const std::string g_PORT = "/dev/ttyUSB0";
const std::string g_PATTERN = "[\\r]{1}([\\d]+)[\\n]{1}";

template <typename T>
void write_series_to_disk(QLineSeries& t)
{
    std::ofstream outfile;
    outfile.open("./csvs/" + QDateTime::currentDateTime().toString().toStdString() + ".csv");
    for (std::size_t i = 0; i < t.count(); ++i)
    {
        QPoint tmp = t.at(i);
        outfile << tmp.x() << ", " << tmp.y() << std::endl;
    }
    outfile.close();
}

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWidget),
    reading_timer(new QTimer(this)),
    handler(SerialcomHandler<concrete_reading>::instance()),
    current_series(new QLineSeries),
    current_chart(new QChart),
    prev_max(X_RANGE), model(new ReadingsModel)
{
    QVXYModelMapper* mapper = new QVXYModelMapper(this);
    mapper->setXColumn(0);
    mapper->setYColumn(1);
    mapper->setSeries(current_series);
    mapper->setModel(model);
    this->setAttribute(Qt::WA_DeleteOnClose, true);
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
    axisX->setLabelFormat("%d");
    current_series->setName("live");
    current_chart->setTitle(current_series->name());
    ui->chart->setRenderHint(QPainter::Antialiasing);
   // axisX->setLabelFormat("%ms");
   // axisX->setTitleText("ms");
   // axisX->setTitleVisible(false);
    axisX->setLabelsVisible();

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, Y_RANGE);
    axisY->setLabelsVisible();
    axisY->setLabelFormat("%d");
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
    connect(ui->save_btn, &QPushButton::clicked, this, &GraphWidget::save_live_graph);

    connect(reading_timer, &QTimer::timeout, this, &GraphWidget::get_readings);
    //connect(ui->back_btn, &QPushButton::clicked, this, &GraphWidget::go_back);
}

GraphWidget::~GraphWidget()
{
    delete ui;
}

void GraphWidget::start()
{
    if (file_thread.joinable())
        file_thread.join();
    current_series->clear();
    reading_timer->start(1);
    //handler.start_sensors();
    concrete_reading::set_start_time();
    handler.start_concrete();


}

void GraphWidget::stop()
{
    reading_timer->stop();
    handler.stop_concrete();
    std::cerr << current_results.size() << std::endl;
}

void GraphWidget::save_live_graph()
{
    //write_series_to_disk(current_series);
}

void GraphWidget::get_readings()
{
    while (handler.available_reads()) {
        concrete_reading&& tmp = handler.read_available();
      //  std::cerr << tmp << ".";
        //current_results.push_back(tmp);
        current_series->append(tmp.p);
      //  readings.push_back(tmp);
        if (tmp.p.x() >= prev_max)
        {
            current_chart->axisX()->setRange(prev_max - 1000, prev_max + 21000);
            prev_max = prev_max + 20000;
        }
       // ui->chart->update();
      //  std::cerr << tmp << std::endl;
    }
}
