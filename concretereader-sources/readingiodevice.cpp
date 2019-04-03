#include "readingiodevice.h"
#include <QLineSeries>

ReadingIODevice::ReadingIODevice(QLineSeries* series, QObject *parent)
    : QIODevice(parent), m_series(series)
{
}

