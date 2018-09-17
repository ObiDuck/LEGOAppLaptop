#include "dbexchange.h"
#include <QtSql>
#include <QtWidgets>

//Надо сделать здесь потом конструктор не забыть

DBExchange::DBExchange(QObject *parent)
{
  m_currentItem = 1;
  hasPicture = false;
  size = 0;
  filtered = false;
}

//получение новых данных текущего итема
void DBExchange::updateView(int id)
{
  QString strQuery;
  QSqlQuery sqlQuery;
  QStringList dataToSend;

  tableModel->setTable(name);
  tableModel->setFilter(QString("id = '%1'").arg(id));
  tableModel->select();
  dataToSend.append(QString("%1").arg(m_currentItem));
  for (int i = 0; i < tableModel->record(0).count(); i++)
    dataToSend.push_back(tableModel->record(0).value(i).toString());
  tableModel->clear();
  emit sendData(dataToSend);
}

int DBExchange::getID(int atID)
{
  if (atID < 0)
    return 0;
  else
  {
    QSqlRecord record;
    tableModel->setTable(name);
    tableModel->select();
    int id;
    while(tableModel->canFetchMore())
      tableModel->fetchMore();
    record = tableModel->record(atID);
    id = record.value("id").toInt();
    tableModel->clear();
    return id;
  }
}

int DBExchange::getSize()
{
  if (filtered)
    return size;
  else
  {
    tableModel->setTable(name);
    tableModel->select();
    while(tableModel->canFetchMore())
      tableModel->fetchMore();
    size = tableModel->rowCount();
    return size;
  }
}

bool DBExchange::setDatabaseConnection(QSqlDatabase database, QString connectionName)
{
  bool ret;
  dbLink_ = QSqlDatabase::cloneDatabase(database,"subcatconnection");
  dbLink_.open();
  ret = dbLink_.isOpen();

  return ret;
}

void DBExchange::Eve(QSqlDatabase &db)
{
  QSqlRecord record;
  QByteArray result;
  tableModel->setTable(name);
  tableModel->select();
  tableModel->removeRow(5, QModelIndex());
  tableModel->clear();
 // tableModel->setTable("Detail");
 /* bool res = tableModel->selectRow(4);
  QString select = tableModel->filter();
  record = tableModel->record(0);
  record = tableModel->record(1);

  record = tableModel->record(2);

  record = tableModel->record(3);

  record = tableModel->record(4);

  record = tableModel->record(5);

  record = tableModel->record(6);
  result = record.value(tableModel->columnCount()-1).toByteArray();
  tableModel->clear();*/
    /*  dbLink_.transaction();
  QString f = "pooopy";
  QSqlRecord record;
  tableModel->setFilter(QString("id IN (SELECT Detail FROM Placement WHERE Color = '%1')").arg(1));
  tableModel->select();
  //QModelIndex ind = tableModel->index(2, 1, QModelIndex());
  for (int i = 0; i < tableModel->rowCount(); i++)
  {
    record = tableModel->record(i);
    QString str = record.value(1).toString();
    int yet = i;
  }
  record.setValue("Name", "Temporary");
  if (tableModel->insertRecord(-1, record))
  //if(tableModel->removeRow(4, QModelIndex()))
  //if(tableModel->setData(ind, "Poop"))
  {
    qDebug()<<"successful insertion";
  }
  else
  {
    f = "nope";
  }
  tableModel->submitAll();
  dbLink_.commit();*/
}

void DBExchange::saveChanges(QStringList dataArray)
{
  dbLink_.transaction();
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  size = tableModel->rowCount();
  int columnCount = hasPicture ? tableModel->columnCount() - 2 : tableModel->columnCount() - 1;
  for (int i = 0; i < columnCount; i++)
  {
    QModelIndex index = tableModel->index(m_currentItem-1, i + 1, QModelIndex());
    tableModel->setData(index, dataArray.at(i));
  }
  tableModel->submitAll();
  dbLink_.commit();
  tableModel->clear();
}

void DBExchange::deleteItem()
{
  QSqlRecord record;
  dbLink_.transaction();
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  tableModel->removeRow(m_currentItem-1, QModelIndex());
  if (m_currentItem > 1)
      m_currentItem--;
  tableModel->submitAll();
  dbLink_.commit();
  size = tableModel->rowCount();
  tableModel->clear();
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  record = tableModel->record(m_currentItem-1);
  updateView(record.value("id").toInt());
}

void DBExchange::addItem()
{
  dbLink_.transaction();
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  QSqlRecord record = tableModel->record();
  record.setValue("Name", "Temporary");
  tableModel->insertRecord(-1, record);
  m_currentItem++;
  tableModel->submitAll();
  dbLink_.commit();
  size = tableModel->rowCount();
  int id;
  record = tableModel->record(m_currentItem-1);
  id = record.value("id").toInt();
  tableModel->clear();
  updateView(id);
}

void DBExchange::nextItem()
{
  QSqlRecord record;
  tableModel->setTable(name);
  tableModel->select();
  int id;
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  int count = tableModel->rowCount();
  if (m_currentItem == count)
    emit sendMsg("End of data reached");
  else
    m_currentItem++;
  record = tableModel->record(m_currentItem-1);
  id = record.value("id").toInt();
  tableModel->clear();
  updateView(id);
}


void DBExchange::jumpID(int newItemID)
{
  tableModel->setTable(name);
  tableModel->setFilter(QString("id = '%1'").arg(newItemID));
  tableModel->select();
  int i = tableModel->rowCount();
  if (i != 0)
  {
    QSqlRecord record;
    int id;
    tableModel->clear();
    tableModel->setTable(name);
    tableModel->select();
    while(tableModel->canFetchMore())
      tableModel->fetchMore();
    i = tableModel->rowCount();
    for (int j = 0; j < i; j++)
    {
      record = tableModel->record(j);
      id = record.value("id").toInt();
      if (id == newItemID)
        m_currentItem = j + 1;
    }
    updateView(newItemID);
  }
  else
  {
    emit sendMsg("Wrong item index");
    QSqlRecord record;
    tableModel->clear();
    tableModel->setTable(name);
    tableModel->select();
    int id;
    while(tableModel->canFetchMore())
      tableModel->fetchMore();
    record = tableModel->record(m_currentItem-1);
    id = record.value("id").toInt();
    tableModel->clear();
     updateView(id);
  }
}
void DBExchange::jumpItem(int newItemID)
{
  QSqlRecord record;
  tableModel->setTable(name);
  tableModel->select();
  int id;
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  int count = tableModel->rowCount() - 1;

  if (newItemID <= 0 || newItemID > count)
  {
    emit sendMsg("Wrong item index");
    if (newItemID <= 0)
    {
        m_currentItem = 1;
        record = tableModel->record(m_currentItem-1);
        id = record.value("id").toInt();
        tableModel->clear();
         updateView(id);
    }
    if (newItemID > count)
    {
        m_currentItem = count;
        record = tableModel->record(m_currentItem-1);
        id = record.value("id").toInt();
        tableModel->clear();
         updateView(id);
    }
    return;
  }

  if (newItemID != m_currentItem)
  {
    m_currentItem = newItemID;   
    record = tableModel->record(m_currentItem-1);
    id = record.value("id").toInt();
    tableModel->clear();
      updateView(id);
  }

}

void DBExchange::prevItem()
{
  if (m_currentItem == 1)
      emit sendMsg("End of data reached");
  else
  {
    QSqlRecord record;
    int id;
    tableModel->setTable(name);
    tableModel->select();
    while(tableModel->canFetchMore())
      tableModel->fetchMore();
      m_currentItem--;
      record = tableModel->record(m_currentItem-1);
      id = record.value("id").toInt();
      updateView(id);
  }
}

void DBExchange::firstItem()
{
  QSqlRecord record;
  int id;
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  m_currentItem = 1;
  record = tableModel->record(m_currentItem-1);
  id = record.value("id").toInt();
  updateView(id);
}

void DBExchange::lastItem()
{
  QSqlRecord record;
  int id;
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  m_currentItem = tableModel->rowCount();
  record = tableModel->record(m_currentItem-1);
  id = record.value("id").toInt();
   tableModel->clear();
  updateView(id);
}

QStringList DBExchange::getName()
{
  QStringList listToSend;
  QSqlRecord record;
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  for (int i = 0; i < tableModel->rowCount(); i++)
  {
    record = tableModel->record(i);
    listToSend.push_back(record.value("Name").toString());
  }
  tableModel->clear();
  return listToSend;
}

QByteArray DBExchange::getIcon(int cur_id)//correct
{
  QStringList dataToSend;
  QByteArray result;
  QSqlRecord record;

  tableModel->setTable(name);
  tableModel->setFilter(QString("id = '%1'").arg(cur_id));
  tableModel->select();
  record = tableModel->record(0);
  result = record.value(tableModel->columnCount()-1).toByteArray();
  tableModel->clear();
  return result;
}

QString DBExchange::getColor(int id)
{
  QString dataToSend;
  QSqlRecord record;

  tableModel->setTable(name);
  tableModel->setFilter(QString("id = '%1'").arg(id));
  tableModel->select();
  record = tableModel->record(0);
  dataToSend = record.value("ColorValue").toString();
  tableModel->clear();

  return dataToSend;
}

QStringList DBExchange::getColorsList()
{
  QStringList list;
  QSqlRecord record;
  tableModel->setTable(name);
  tableModel->setFilter(QString("Family != '%1' AND Family != '%2' AND Family != '%3'").arg(1).arg(2).arg(13));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  int yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("ColorValue").toString());
  }
  tableModel->clear();

  tableModel->setTable(name);
  tableModel->setFilter(QString("Family = '%1'").arg(1));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("ColorValue").toString());
  }
  tableModel->clear();


  tableModel->setTable(name);
  tableModel->setFilter(QString("Family = '%1'").arg(2));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("ColorValue").toString());
  }
  tableModel->clear();


  tableModel->setTable(name);
  tableModel->setFilter(QString("Family = '%1'").arg(13));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("ColorValue").toString());
  }
  tableModel->clear();
  return list;
}

QStringList DBExchange::getColorsIdList()
{
  QStringList list;
  QSqlRecord record;
  tableModel->setTable(name);
  tableModel->setFilter(QString("Family != '%1' AND Family != '%2' AND Family != '%3'").arg(1).arg(2).arg(13));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  int yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("id").toString());
  }
  tableModel->clear();

  tableModel->setTable(name);
  tableModel->setFilter(QString("Family = '%1'").arg(1));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
   list.push_back(record.value("id").toString());
    }
  tableModel->clear();


  tableModel->setTable(name);
  tableModel->setFilter(QString("Family = '%1'").arg(2));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("id").toString());
  }
  tableModel->clear();


  tableModel->setTable(name);
  tableModel->setFilter(QString("Family = '%1'").arg(13));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("id").toString());
   }
  tableModel->clear();
  return list;
}

QStringList DBExchange::getIDs()
{
  QStringList list;
  QSqlRecord record;
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  int yet = tableModel->rowCount();
  for (int i = 0; i < yet; i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("id").toString());
  }
  tableModel->clear();
  return list;
}

QString DBExchange::getFamilyColor(int id)
{    
  QString dataToSend;
  QSqlRecord record;
  tableModel->setTable(name);
  tableModel->setFilter(QString("id = '%1'").arg(id));
  tableModel->select();
  record = tableModel->record(0);
  dataToSend = record.value("Family").toString();
  tableModel->clear();
  if (dataToSend == "1") dataToSend = "T";
  else if (dataToSend == "2") dataToSend = "M";
  else if (dataToSend == "13") dataToSend = "L";
  else dataToSend = "";

  return dataToSend;
}

QString DBExchange::getFamilyColor(QString id)
{
  QSqlRecord record;
  QString dataToSend;

  tableModel->setTable(name);
  tableModel->setFilter(QString("ColorValue = \"%1\"").arg(id));
  tableModel->select();
  record = tableModel->record(0);
  dataToSend = record.value("Family").toString();
  tableModel->clear();
  if (dataToSend == "1") dataToSend = "T";
  else if (dataToSend == "2") dataToSend = "M";
  else if (dataToSend == "13") dataToSend = "L";
  else dataToSend = "";

  return dataToSend;
}

void DBExchange::getData()
{
  QSqlRecord record;
  int id;
  m_currentItem = 1;
  tableModel->setTable(name);
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  record = tableModel->record(m_currentItem-1);
  id = record.value("id").toInt();
    tableModel->clear();
  updateView(id);
}

void DBExchange::setIcon(QString iconPath)
{
  QByteArray bArray;
  QImage img;
  QPixmap pxmap;
  QString strQuery;
  QBuffer buf;
  QSqlQuery sqlQuery;
  QSqlError err;
  bool flag;
  QSqlRecord record;

  tableModel->setTable(name);
  dbLink_.transaction();
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  iconPath.remove(0,8);
  img.load(iconPath);
  pxmap.convertFromImage(img);
  buf.setBuffer(&bArray);
  buf.open(QIODevice::WriteOnly);
  pxmap.save(&buf, "PNG");
  buf.close();

  QModelIndex index = tableModel->index(m_currentItem-1, tableModel->columnCount()-1, QModelIndex());
  tableModel->setData(index, bArray);
  tableModel->submitAll();
  dbLink_.commit();
  int id;
  record = tableModel->record(m_currentItem-1);
  id = record.value("id").toInt();
  tableModel->clear();
  updateView(id);
}

void DBExchange::applyFilter(int catFilter, int colorFilter, QString placeFilter, QStringList& list)
{
  QString strQuery, boxIndx, rowIndx, columnIndx;
  QSqlQuery sqlQuery;
  filtered = true;
  tableModel->setTable(name);
  if ((catFilter != 0) && (colorFilter == 0) && (placeFilter == "0/0/0"))
    tableModel->setFilter(QString("SubCategory = '%1'").arg(catFilter));
  if ((catFilter == 0) && (colorFilter != 0) && (placeFilter == "0/0/0"))
    tableModel->setFilter(QString("id IN (SELECT Detail FROM Placement WHERE Color = '%1')").arg(colorFilter));
  if ((catFilter != 0) && (colorFilter != 0) && (placeFilter == "0/0/0"))
    tableModel->setFilter(QString("SubCategory = '%1' AND id IN (SELECT Detail FROM Placement WHERE Color = '%2')").arg(catFilter).arg(colorFilter));
  if ((catFilter == 0) && (colorFilter == 0) && (placeFilter != "0/0/0"))
  {
    boxIndx = placeFilter.section("/", 0, 0);
    rowIndx = placeFilter.section("/", 1, 1);
    columnIndx = placeFilter.section("/", 2);
    tableModel->setFilter(QString("id IN (SELECT Detail FROM Placement WHERE BoxIndex = '%1' AND RowIndex = '%2' AND ColumnIndex = '%3')").arg(boxIndx).arg(rowIndx).arg(columnIndx));
  }

  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  QSqlRecord record;
  for (int i = 0; i < tableModel->rowCount(); i++)
  {
    record = tableModel->record(i);
    list.push_back(record.value("id").toString());
  }
  size = tableModel->rowCount();
  tableModel->clear();
}

void DBExchange::setName(QString tableName)
{
  name = tableName;
  tableModel->setTable(name);
}

QList<QStringList> DBExchange::getInfo(int idDet)
{
  QString strQuery;
  QSqlQuery sqlQuery;
  QString res;
  QStringList data;
  QList<QStringList> modelProto;
  //strQuery1 = QString("SELECT Color, BoxIndex, RowIndex, ColumnIndex, Count, id FROM Placement WHERE Detail = '%1'").arg(m_id.at(idDet-1));
  tableModel->setTable(name);
  tableModel->setFilter(QString("Detail = '%1'").arg(idDet));
  tableModel->select();
  while(tableModel->canFetchMore())
    tableModel->fetchMore();
  for (int i = 0; i < tableModel->rowCount(); i++)
  {
    QSqlRecord record;
    record = tableModel->record(i);
    data.append(record.value("Color").toString());
    data.append(record.value("BoxIndex").toString());
    data.append(record.value("RowIndex").toString());
    data.append(record.value("ColumnIndex").toString());
    data.append(record.value("Count").toString());
    data.append(record.value("id").toString());
    res = data.at(0);
    int index;
    index = res.toInt();
    strQuery = QString("SELECT ColorValue FROM Color WHERE id = %1").arg(index);
    sqlQuery.exec(strQuery);
    sqlQuery.next();
    data.replace(0, sqlQuery.value(0).toString());
    modelProto.append(data);
    data.clear();
  }
  sqlQuery.clear();
  tableModel->clear();
  return modelProto;
}
