#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class GraphWidget;
class MenuWidget;

class settings_t
{
public:
    void            setInterval(unsigned short i) { interval = i; }
    void            setXmax(unsigned short x) { x_max = x; }
    void            setYmax(unsigned short y) { y_max = y; }
    void            setBuffer(unsigned short b) { buffer = b; }
    unsigned short  getInterval() { return interval; }
    unsigned short  getXmax() { return x_max; }
    unsigned short  getYmax() { return y_max; }
    unsigned short  getBuffer() { return buffer; }
private:
    unsigned short  interval = 40;
    unsigned short  x_max = 180;
    unsigned short  y_max = 4100;
    unsigned short  buffer = 32;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static settings_t user_settings;
public slots:
        void show_graph();
        void show_menu();        
private:
    Ui::MainWindow *ui;

    //GraphWidget *graph;
 //   MenuWidget *menu;
};

#endif // MAINWINDOW_H
