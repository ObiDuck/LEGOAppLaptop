#ifndef DBEXCHANGE_H
#define DBEXCHANGE_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QPixmap>
#include <QSqlQuery>
#include <QSqlTableModel>

class DBExchange : public QObject
{
  Q_OBJECT
public:
  explicit DBExchange(QObject *parent = 0);
  void updateView(int id);
  bool setDatabaseConnection(QSqlDatabase database, QString connectionName);
  QSqlTableModel* tableModel;
  bool hasPicture;
  void Eve(QSqlDatabase &db);
  void setName(QString tableName);
  int getSize();
  QStringList getIDs();
signals:
  void sendData(QStringList array);
  void sendMsg(QString newText);
  void sendByteArray(QString picArray);
public slots:
  int getID(int atID);
  QStringList getName();
  QByteArray getIcon(int cur_id);
  void setIcon(QString iconPath);
  QString getColor(int id);
  QStringList getColorsList();
  QString getFamilyColor(int id);
  QString getFamilyColor(QString id);
  void getData();
  void lastItem();
  void firstItem();
  void prevItem();
  void nextItem();
  void jumpItem(int newItemID);
  void jumpID(int newItemID);
  void deleteItem();
  void addItem();
  void saveChanges(QStringList dataArray);
  QList<QStringList> getInfo(int idDet);
  void applyFilter(int catFilter, int colorFilter, QString placeFilter, QStringList& list);
private:
  int size;
  bool filtered;
  QString name;
  QSqlDatabase dbLink_;
  int m_currentItem;
  QSqlQuery currentQuery_;
  QString m_linkedTable;
  QString m_tableName;
  QStringList m_tableItems;
  QString sql_getDataForView();
  QString sql_add();
};

#endif // DBEXCHANGE_H
