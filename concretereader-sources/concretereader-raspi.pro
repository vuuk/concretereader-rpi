#-------------------------------------------------
#
# Project created by QtCreator 2019-04-02T16:54:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets charts

TARGET = concretereader-raspi
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++14

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    graphwidget.cpp \
    concrete_reading.cpp \
    menuwidget.cpp \
    readingsmodel.cpp

HEADERS += \
        mainwindow.h \
    graphwidget.h \
    serialcomhandler.h \
    concrete_reading.h \
    menuwidget.h \
    readingsmodel.h

FORMS += \
        mainwindow.ui \
    graphwidget.ui \
    menuwidget.ui

INCLUDEPATH += /home/kirk/Boost/boost_1_64_0/

LIBS += -L/home/kirk/Boost/boost_1_64_0/stage/lib/ -lboost_system
LIBS += -L/home/kirk/Boost/boost_1_64_0/stage/lib/ -lboost_filesystem
LIBS += -L/home/kirk/Boost/boost_1_64_0/stage/lib/ -lboost_thread


# Default rules for deployment.
target.path = /home/pi
INSTALLS += target

