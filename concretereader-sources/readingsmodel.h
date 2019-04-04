#ifndef READINGSMODEL_H
#define READINGSMODEL_H

#include <QAbstractTableModel>
#define NUM_OF_LINES 4

class ReadingsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ReadingsModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // READINGSMODEL_H
