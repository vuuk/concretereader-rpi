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
#include "mainwindow.h"
#include <thread>
#include <algorithm>
const std::string g_PORT = "/dev/ttyUSB0";
//const std::string g_PATTERN = "[\\r]{1}([\\d]+)[\\n]{1}";
const std::string g_PATTERN = "[\\n]{1}([\\d]+)[\\r]{1}";

//template <typename T>
void write_series_to_disk(QVector<concrete_reading>& dq)
{
    std::ofstream outfile;
    std::string target_folder = "./csvs/";
    std::string file_name = QDateTime::currentDateTime().toString().toStdString();
    std::remove(file_name.begin(), file_name.end(), '?');
    std::remove(file_name.begin(), file_name.end(), ':');
    std::remove(file_name.begin(), file_name.end(), ' ');
    std::remove(file_name.begin(), file_name.end(), '.');
    outfile.open(target_folder + file_name + ".csv");
    auto count = dq.size();
    for (concrete_reading r : dq) {
        outfile << r << std::endl;
    }
    outfile.close();
    qDebug() << count << " concrete readings written to disk with size: " << sizeof(concrete_reading) << " totalling " << sizeof(concrete_reading) * count;
}

GraphWidget::GraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GraphWidget),
    reading_timer(new QTimer(this)),
    clock_timer(new QTimer(this)),
    handler(SerialcomHandler<concrete_reading>::instance()),
    current_series(new QLineSeries),
    current_chart(new QChart),
    prev_max(MainWindow::user_settings.getXmax())
{
    readings.reserve(15000);

    this->setAttribute(Qt::WA_DeleteOnClose, true);
    QFont stnd;
    stnd.setPointSize(7);

    ui->setupUi(this);

    handler.setPortName(g_PORT);
    handler.setRegexPattern(g_PATTERN);
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, prev_max);
    axisX->setLabelsFont(stnd);
    axisX->setLabelFormat("%d s");

    current_chart->setTitle(current_series->name());
    ui->chart->setRenderHint(QPainter::Antialiasing);

    axisX->setLabelsVisible();

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, MainWindow::user_settings.getYmax());
    axisY->setLabelFormat("%d");
    axisY->setLabelsVisible();
    axisY->setLabelsFont(stnd);

    current_chart->setContentsMargins(0,0,0,0);

    current_chart->addSeries(current_series);
    current_chart->setAxisX(axisX, current_series);
    current_chart->setAxisY(axisY, current_series);
    current_chart->legend()->hide();

    ui->chart->setChart(current_chart);
    connect(ui->start_btn, &QPushButton::clicked, this, &GraphWidget::start);
    connect(ui->stop_btn, &QPushButton::clicked, this, &GraphWidget::stop);

    connect(reading_timer, &QTimer::timeout, this, &GraphWidget::get_readings);
    connect(clock_timer, &QTimer::timeout, this, &GraphWidget::tick_clock);

}

GraphWidget::~GraphWidget()
{
    delete ui;
}

void GraphWidget::start()
{
    if (!handler.check_port())
    {
        QMessageBox messageBox;
        messageBox.setText("No USB Device found!");
        messageBox.setFixedSize(500,200);
        messageBox.exec();
       // handler.close();
        return;
    }
    ui->start_btn->setDisabled(true);
    ui->export_btn->setDisabled(true);
    ui->stop_btn->setDisabled(false);
    readings.clear();
    readings.reserve(15000);
    current_series->clear();
    current_chart->axisY()->setRange(0, MainWindow::user_settings.getYmax());
    current_chart->axisX()->setRange(0, MainWindow::user_settings.getXmax());
    reset_clock();
    max_val.setY(0);
    total_val.setX(0);
    total_val.setY(0);
    ui->max_lcdNumber->display(max_val.y());
    ui->avg_lcdNumber->display(max_val.y());
    handler.start_concrete();

    ui->avg_lcdNumber->setDigitCount(5);
    ui->max_lcdNumber->setDigitCount(5);
    ui->time_lcdNumber->setDigitCount(3);
    ui->time_lcdNumber->display(0);

    reading_timer->start(MainWindow::user_settings.getInterval()*0.90);
    clock_timer->start(1000);
    prev_max = MainWindow::user_settings.getXmax();
    concrete_reading::set_start_time();
     ui->avg_lcdNumber->display(0.0);

}

void GraphWidget::stop()
{
    reading_timer->stop();
    clock_timer->stop();
    handler.stop_concrete();
    write_series_to_disk(readings);
    ui->start_btn->setDisabled(false);
    ui->export_btn->setDisabled(false);
    ui->stop_btn->setDisabled(true);
}


void GraphWidget::save_live_graph()
{
  write_series_to_disk(readings);
}

void GraphWidget::get_readings()
{


    while (handler.available_reads()) {
        concrete_reading tmp = handler.read_available();

        current_series->append(tmp.X(), tmp.Y());

        double x_tmp = tmp.x;
        int y_tmp = tmp.y;

        readings.push_back(tmp);

        if (y_tmp > 0) {
            double avg = 0.0;
            total_val.setX(total_val.x() + 1);
            total_val.setY((total_val.y() + y_tmp));
            avg = total_val.y() / total_val.x();
            ui->avg_lcdNumber->display(avg);
        }

        if (x_tmp >= prev_max)
        {

            current_series->removePoints(0, current_series->count()-1000);
            const short min = current_series->at(0).x();
            const short  max = prev_max + (MainWindow::user_settings.getXmax() * 0.9);
            current_chart->axisX()->setRange(min, max);
            prev_max = prev_max + MainWindow::user_settings.getXmax();

            qDebug() <<  "MIN: " << min << "MAX: " << max;
       }
        if (y_tmp > max_val.y())
        {
            max_val.setY(y_tmp);
            ui->max_lcdNumber->display(max_val.y());
            if (y_tmp >= MainWindow::user_settings.getYmax())
                current_chart->axisY()->setRange(0, int(y_tmp*1.25));
        }

    }
}

void GraphWidget::tick_clock()
{
    ui->time_lcdNumber->display(concrete_reading::secs_since_start_time());
}

void GraphWidget::update_vals()
{

}

void GraphWidget::on_export_btn_clicked()
{
   // qDebug() << QFileDialog::getSaveFileName(this, tr("Save File"), "./test.png",
            //                                 tr("Images (*.csv)"));
    QDir usb_dir("/media/usb/");
    if (!usb_dir.exists())
        return;
    QString dest = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                   "/media/usb",
                                                   QFileDialog::ShowDirsOnly
                                                   | QFileDialog::DontResolveSymlinks);
    QString src = "/home/pi/csvs/";
    QDir src_dir(src), dst(dest);
    if (!src_dir.exists() || !dst.exists())
    {
        return;
    }
    auto files = src_dir.entryList(QStringList() << "*.csv");
    for (auto file : files)
    {
        if (!dst.exists())
            break;
        QFile tmp(src + file);
        tmp.setPermissions(QFileDevice::WriteOther);
        if (tmp.exists())
        {
            qDebug() << "File to move: " << tmp.fileName();
        }
       auto res = tmp.rename(dest + "/" + file);
       if (res){
            qDebug() << "Wrote file: " << dest << "/" << file;
        }
        else {
            qDebug() << "Failed to write file: " << dest + "/" + file;
        }
    }


}
