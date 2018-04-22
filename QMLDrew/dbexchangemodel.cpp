#include "dbexchangemodel.h"

DBExchangeModel::DBExchangeModel(QObject *parent)
    : QAbstractListModel(parent)
{
}//+

void DBExchangeModel::addData(const QStringList &data)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_data << data;
    endInsertRows();
}

void DBExchangeModel::removeData()
{
    beginRemoveRows(QModelIndex(), 0, rowCount());
    m_data.clear();
    endRemoveRows();
}

int DBExchangeModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_data.count();
}

QVariant DBExchangeModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.count())
        return QVariant();

    const QStringList &data = m_data.at(index.row());
    for (int i = 1; i <= data.size(); i++)
        if (role == Qt::UserRole + i)
            return data.at(i - 1);

    return QVariant();
}

void DBExchangeModel::setRoles(QStringList roleName)
{
    for (int i = 0; i < roleName.size(); i++)
        m_roleNames.append(roleName.at(i).toLower());
}

QHash<int, QByteArray> DBExchangeModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    for (int i = 1; i <= m_roleNames.size(); i++)
    {
        roles[Qt::UserRole + i] = m_roleNames.at(i - 1).toUtf8();
    }
    return roles;
}
