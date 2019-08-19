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
        return size_of_largest();

    // FIXME: Implement me!
}

int ReadingsModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return m_loaded_readings.size();

    // FIXME: Implement me!
}

QVariant ReadingsModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    if (role == Qt::DisplayRole)
    {
        if (index.column() < m_loaded_readings.size() && index.column() >= 0)
        {
            if (m_loaded_readings.at(index.column()).size() <= index.row())
                return m_loaded_readings.at(index.column()).at(index.row());
        }
    }
    return QVariant();
}

QVector<QPoint>::size_type ReadingsModel::size_of_largest() const
{
    QVector<QPoint>::size_type max = 0;
    for (auto &v : m_loaded_readings)
        if (v.size() > max)
            max = v.size();
    return max;


}
