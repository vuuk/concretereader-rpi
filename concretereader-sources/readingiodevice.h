#ifndef READINGIODEVICE_H
#define READINGIODEVICE_H

#include <QAbstractItemModel>
#include <QIODevice>

class QLineSeries;

class ReadingIODevice : public QIODevice
{
    Q_OBJECT

public:
    explicit ReadingIODevice(QLineSeries* series, QObject *parent = nullptr);

private:
    QLineSeries* m_series;
};

#endif // READINGIODEVICE_H
