#ifndef DBEXCHANGE_H
#define DBEXCHANGE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QPixmap>
#include <QSqlQuery>

class DBExchange : public QObject
{
    Q_OBJECT
public:
    explicit DBExchange(QObject *parent = 0);
    void setTable(QString tableName, QStringList tableItems);
    void updateView();    
    void getItemsID();
    int getCount();
signals:
    void sendData(QStringList array);
    void sendMsg(QString newText);
    void sendByteArray(QString picArray);
public slots:
    QStringList getName();
    QByteArray getIcon(int cur_id);
    void setIcon(QString iconPath);
    QString getColor(int id);
    QString getFamilyColor(int id);
    QString getFamilyColor(QString id);
    int getID(int atID);
    void getData();
    void lastItem();
    void firstItem();
    void prevItem();
    void nextItem();
    void jumpItem(int newItemID);
    void deleteItem();
    void replaceItem(int newID);
    void addItem();
    void saveChanges(QStringList dataArray);
    QList<QStringList> getInfo(int idDet);
    void applyFilter(int catFilter, int colorFilter, QString placeFilter);
private:
    int m_currentItem;
    QSqlQuery currentQuery_;
    QString m_linkedTable;
    QString m_tableName;
    QStringList m_tableItems;
    QString sql_getDataForView();
    QString sql_updateChanges(QStringList dataArray);
    QString sql_delete();
    QString sql_getItemsID();
    QString sql_add();
    QString sql_replace(int newID);
    QVector<int> m_id;
};

#endif // DBEXCHANGE_H
