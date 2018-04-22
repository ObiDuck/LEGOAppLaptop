#ifndef DBEXCHANGEMODEL_H
#define DBEXCHANGEMODEL_H

#include <QObject>
#include <QAbstractListModel>

//класс для модели хранения информации о детали
class DBExchangeModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DBExchangeModel(QObject* parent = 0);

    void addData(const QStringList &data);
    void removeData();
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    void setRoles(QStringList roleName);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<QStringList> m_data;
    QStringList m_roleNames;
};


#endif // DBEXCHANGEMODEL_H
