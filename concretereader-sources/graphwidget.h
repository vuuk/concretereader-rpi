#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>

#include <QtCharts>

#include <QVector>
#include <QTimer>
#include <QThread>
#include <deque>
#include "serialcomhandler.h"
#include "concrete_reading.h"
#define X_RANGE 20000
#define Y_RANGE 4100
namespace Ui {
class GraphWidget;
}

extern const std::string g_PORT;
extern const std::string g_PATTERN;
class ReadingsModel;
class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    ~GraphWidget();
public slots:
    void start();
    void stop();
    void save_live_graph();
private slots:
    void get_readings();
private:
private:
    Ui::GraphWidget *ui;
    QVector<concrete_reading> current_results;
    QTimer *reading_timer;
    SerialcomHandler<concrete_reading> &handler;
    QLineSeries* current_series;
    QChart* current_chart;
    std::thread file_thread;
    std::deque<concrete_reading> readings;
    long long prev_max;
    ReadingsModel* model;
};

#endif // GRAPHWIDGET_H
