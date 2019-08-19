#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QWidget>

#include <QtCharts>

#include <QVector>
#include <QTimer>
#include <QThread>
#include "menuwidget.h"
#include <deque>
#include <memory>
#include "serialcomhandler.h"
#include "concrete_reading.h"
//#define X_RANGE 3000
#define X_RANGE 120
#define Y_RANGE 200
namespace Ui {
class GraphWidget;
}

extern const std::string g_PORT;
extern const std::string g_PATTERN;
//class ReadingsModel;
//class MenuWidget;
class GraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GraphWidget(QWidget *parent = nullptr);
    void reset_clock() { prev_sec = 0; }
    ~GraphWidget();
public slots:
    void start();
    void stop();
    void save_live_graph();
private slots:
    void get_readings();
    void tick_clock();
    void update_vals();
    void on_export_btn_clicked();
private:
private:
    Ui::GraphWidget *ui;
   // QVector<concrete_reading> current_results;
    QTimer *reading_timer;
    QTimer *clock_timer;
    SerialcomHandler<concrete_reading> &handler;
    QLineSeries* current_series;
    QChart* current_chart;
 //   std::thread file_thread;
    QPoint max_val;
    QPoint total_val;
    QVector<concrete_reading> readings;
    int prev_max;
 //   MenuWidget* menu;
    double prev_sec = 0.0;
    QThread* drawing_thread;
   // unsigned short x_range = 300;
   // unsigned short y_range = 4095;
   // unsigned short update_freq = 1;
};

#endif // GRAPHWIDGET_H
