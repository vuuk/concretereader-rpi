#include "readingsmodel.h"

ReadingsModel::ReadingsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ReadingsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!
}

int ReadingsModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int ReadingsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 2;

    // FIXME: Implement me!
}

QVariant ReadingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
