#include "dbexchange.h"
#include <QtSql>
#include <QtWidgets>

//Надо сделать здесь потом конструктор не забыть

DBExchange::DBExchange(QObject *parent)
{
    m_currentItem = 1;
    getItemsID();
}

void DBExchange::setTable(QString tableName, QStringList tableItems)
{
    m_tableItems.clear();
    m_tableName = tableName;
    for (int i = 0; i < tableItems.size(); i++)
        m_tableItems.push_back(tableItems.at(i));

    getItemsID();
}

int DBExchange::getCount()
{
    return m_id.size();
}

int DBExchange::getID(int atID)
{
    if (atID - 1 < 0)
        return 0;
    else
        return m_id.at(atID - 1);
}

void DBExchange::getItemsID()
{
    QString strQuery;
    QSqlQuery sqlQuery;

    m_id.clear();
    strQuery = sql_getItemsID();
    sqlQuery.exec(strQuery);
    while (sqlQuery.next())
        m_id.push_back(sqlQuery.value(0).toInt());
    sqlQuery.clear();
}

QString DBExchange::sql_getItemsID()
{
    QString strQuery;
    strQuery.append("SELECT id FROM ");
    strQuery.append(m_tableName);

    return strQuery;
}


//формирование строки запроса для получения нововых данных текущего итема
QString DBExchange::sql_getDataForView()
{
    QString strQuery;
    strQuery.append("SELECT ");
    for (int i = 0; i < m_tableItems.size() - 1; i++)
    {
        strQuery.append(m_tableItems.at(i));
        strQuery.append(", ");
    }
    strQuery.append(m_tableItems.at(m_tableItems.size() - 1));
    strQuery.append(" FROM ");
    strQuery.append(m_tableName);
    strQuery.append(QString(" WHERE id = %1").arg(m_id.at(m_currentItem-1)));

    return strQuery;
}

//получение новых данных текущего итема
void DBExchange::updateView()
{
  QString strQuery;
  QSqlQuery sqlQuery;
  QStringList dataToSend;

  dataToSend.push_back(QString("%1").arg((m_currentItem)));
  strQuery = sql_getDataForView();
  sqlQuery.exec(strQuery);
  //currentQuery_.exec(strQUery);
  if (sqlQuery.next())
  //if (currentQuery.next())
  {
    for (int i = 0; i < m_tableItems.size(); i++)
      dataToSend.push_back(sqlQuery.value(i).toString());
  //    dataToSend.push_back(currentQuery_.value(i).toString());
  }
  sqlQuery.clear();

  emit sendData(dataToSend);
}

//формируем строку для запроса в базу для обновления в бд с формы
QString DBExchange::sql_updateChanges(QStringList dataArray)
{
    QString strQuery;

    strQuery.append("UPDATE ");
    strQuery.append(m_tableName);
    strQuery.append(" SET ");
    for (int i = 0; i < m_tableItems.size() - 1; i++)
    {
        strQuery.append(m_tableItems.at(i));//Name/Category/Icon
        strQuery.append(" = '");
        strQuery.append(dataArray.at(i));
        strQuery.append("', ");
    }
    if ((m_tableItems.last() != "Icon")&&(m_tableItems.last() != "Picture"))
    {
        strQuery.append(m_tableItems.at(m_tableItems.size() - 1));//Name/Category/Icon
        strQuery.append(" = '");
        strQuery.append(dataArray.at(m_tableItems.size() - 1));
        strQuery.append("'");
    }
    else {
        int i = strQuery.size();
        if (i > 0)
            strQuery.remove(i - 2, 2);
    }

    strQuery.append(" WHERE id = ");
    strQuery.append(QString("%1").arg(m_id.at(m_currentItem - 1)));

    return strQuery;
}
//получаем список данных из qml о том, как заполнили форму и обновляем БД в соответствии с этим
void DBExchange::saveChanges(QStringList dataArray)
{
    QString strQuery;
    QSqlQuery sqlQuery;
    QSqlError err;
    bool flag;

    strQuery = sql_updateChanges(dataArray);
    flag = sqlQuery.exec(strQuery);
    if (!flag)
        err = sqlQuery.lastError();
    sqlQuery.clear();
}

QString DBExchange::sql_delete()
{
    QString strQuery;

    strQuery.append("DELETE FROM ");
    strQuery.append(m_tableName);
    strQuery.append(" WHERE id = ");
    strQuery.append(QString("%1").arg(m_id.at(m_currentItem - 1)));

    return strQuery;
}

QString DBExchange::sql_replace(int newID)
{//????
    QString strQuery;
    strQuery.append("UPDATE ");
    strQuery.append(m_linkedTable);
    strQuery.append(" SET ");
    strQuery.append(m_tableName);
    strQuery.append(" = ");
    strQuery.append(newID);
    strQuery.append(" WHERE ");
    strQuery.append(m_tableName);
    strQuery.append(" = ");
    strQuery.append(QString("%1").arg(m_id.at(m_currentItem - 1)));

    return strQuery;
}

void DBExchange::replaceItem(int newID)
{
    QString strQuery;
    QSqlQuery sqlQuery;

    strQuery = sql_replace(newID);
    sqlQuery.exec(strQuery);
    deleteItem();
    sqlQuery.clear();
}

void DBExchange::deleteItem()
{
    QString strQuery;
    QSqlQuery sqlQuery;

    strQuery = sql_delete();
    sqlQuery.exec(strQuery);
    m_id.remove(m_currentItem - 1);
    if (m_currentItem > 1)
        m_currentItem--;
    getItemsID();
    updateView();
    sqlQuery.clear();
}


QString DBExchange::sql_add()
{
    QString strQuery;

    strQuery.append("INSERT INTO ");
    strQuery.append(m_tableName);
    strQuery.append(" (");    
    strQuery.append(m_tableItems.at(0));
    strQuery.append(") VALUES (1)");

    return strQuery;
}

void DBExchange::addItem()
{
    QString strQuery;
    QSqlQuery sqlQuery;

    strQuery = sql_add();
    sqlQuery.exec(strQuery);
    getItemsID();
    m_currentItem = m_id.size();
    updateView();
    sqlQuery.clear();
}

void DBExchange::nextItem()
{
    if (m_currentItem == m_id.size())
        emit sendMsg("End of data reached");
    else
    {
        m_currentItem++;
        updateView();
    }
}

void DBExchange::jumpItem(int newItemID)
{
    if (newItemID <= 0 || newItemID > m_id.size())
    {
        emit sendMsg("Wrong item index");
        if (newItemID <= 0)
        {
            m_currentItem = 1;
            updateView();
        }
        if (newItemID > m_id.size())
        {
            m_currentItem = m_id.size();
            updateView();
        }

        return;
    }

    if (newItemID != m_currentItem)
    {
        m_currentItem = newItemID;
        updateView();
    }
}

void DBExchange::prevItem()
{
    if (m_currentItem == 1)
        emit sendMsg("End of data reached");
    else
    {
        m_currentItem--;
        updateView();
    }
}

void DBExchange::firstItem()
{
    m_currentItem = 1;
    updateView();
}

void DBExchange::lastItem()
{
    m_currentItem = m_id.size();
    updateView();
}

QStringList DBExchange::getName()
{
    QString strQuery;
    QSqlQuery sqlQuery;
    QStringList listToSend;

    strQuery.append("SELECT Name FROM ");
    strQuery.append(m_tableName);

    sqlQuery.exec(strQuery);
    while (sqlQuery.next())
    {
       listToSend.push_back(sqlQuery.value(0).toString());
    }
    sqlQuery.clear();

    return listToSend;
}

QByteArray DBExchange::getIcon(int cur_id)
{
    QString strQuery;
    QSqlQuery sqlQuery;
    QStringList dataToSend;
    QByteArray result;

    strQuery.append("SELECT ");
    strQuery.append(m_tableItems.last());
    strQuery.append(" FROM ");
    strQuery.append(m_tableName);
    strQuery.append(" WHERE id = ");
    strQuery.append(QString("%1").arg(m_id.at(cur_id-1)));

    sqlQuery.exec(strQuery);

    if (sqlQuery.next())
    {
       result = (sqlQuery.value(0).toByteArray());
    }
    sqlQuery.clear();
    return result;
}

QString DBExchange::getColor(int id)
{
    QString strQuery;
    QSqlQuery sqlQuery;
    QString dataToSend;

    strQuery.append(QString("SELECT ColorValue FROM Color WHERE id = %1").arg(id));
    sqlQuery.exec(strQuery);

    while (sqlQuery.next())
    {
       dataToSend.push_back(sqlQuery.value(0).toString());
    }
    sqlQuery.clear();
    return dataToSend;
}

QString DBExchange::getFamilyColor(int id)
{
    QString strQuery;
    QSqlQuery sqlQuery;
    QString dataToSend = "";
    if (id >= 0) {
    strQuery.append(QString("SELECT Family FROM Color WHERE id = %1").arg(m_id.at(id)));
    sqlQuery.exec(strQuery);

    while (sqlQuery.next())
    {
       dataToSend.push_back(sqlQuery.value(0).toString());
    }
    sqlQuery.clear();

    if (dataToSend == "1") dataToSend = "T";
    else if (dataToSend == "2") dataToSend = "M";
    else dataToSend = "";
    }
    return dataToSend;
}

QString DBExchange::getFamilyColor(QString id)
{
    QString strQuery;
    QSqlQuery sqlQuery;
    QString dataToSend = "";

    strQuery.append(QString("SELECT Family FROM Color WHERE ColorValue = \"%1\"").arg(id));
    sqlQuery.exec(strQuery);

    while (sqlQuery.next())
    {
       dataToSend.push_back(sqlQuery.value(0).toString());
    }
    sqlQuery.clear();

    if (dataToSend == "1") dataToSend = "T";
    else if (dataToSend == "2") dataToSend = "M";
    else dataToSend = "";

    return dataToSend;
}

void DBExchange::getData()
{
    m_currentItem = 1;
    updateView();
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

    iconPath.remove(0,8);
    img.load(iconPath);
    pxmap.convertFromImage(img);
    buf.setBuffer(&bArray);
    buf.open(QIODevice::WriteOnly);
    pxmap.save(&buf, "PNG");
    buf.close();

    strQuery.append("UPDATE ");
    strQuery.append(m_tableName);
    strQuery.append(" SET ");
    strQuery.append(m_tableItems.at(m_tableItems.size() - 1));//Name/Category/Icon
    strQuery.append(" = :picture WHERE id = :id");

    sqlQuery.prepare(strQuery);
    sqlQuery.bindValue(":picture", bArray);
    sqlQuery.bindValue(":id", m_id.at(m_currentItem - 1));
    flag = sqlQuery.exec();
    if (!flag)
        err = sqlQuery.lastError();
    sqlQuery.clear();
}


void DBExchange::applyFilter(int catFilter, int colorFilter, QString placeFilter)
{
    QString strQuery, boxIndx, rowIndx, columnIndx;
    QSqlQuery sqlQuery;

    m_id.clear();

    if ((catFilter == 0) && (colorFilter == 0) && (placeFilter == "0/0/0"))
        strQuery = QString("SELECT id FROM Detail");
    if ((catFilter != 0) && (colorFilter == 0) && (placeFilter == "0/0/0"))
        strQuery = QString("SELECT id FROM Detail WHERE SubCategory = '%1'").arg(catFilter);
    if ((catFilter == 0) && (colorFilter != 0) && (placeFilter == "0/0/0"))
         strQuery = QString("SELECT id FROM Detail WHERE id IN (SELECT Detail FROM Placement WHERE Color = '%1')").arg(colorFilter);
    if ((catFilter != 0) && (colorFilter != 0) && (placeFilter == "0/0/0"))
        strQuery = QString("SELECT id FROM Detail WHERE SubCategory = '%1' AND id IN (SELECT Detail FROM Placement WHERE Color = '%2')").arg(catFilter).arg(colorFilter);
    if ((catFilter == 0) && (colorFilter == 0) && (placeFilter != "0/0/0"))
    {
        boxIndx = placeFilter.section("/", 0, 0);
        rowIndx = placeFilter.section("/", 1, 1);
        columnIndx = placeFilter.section("/", 2);
        strQuery = QString("SELECT id FROM Detail WHERE id IN (SELECT Detail FROM Placement WHERE BoxIndex = '%1' AND RowIndex = '%2' AND ColumnIndex = '%3')").arg(boxIndx).arg(rowIndx).arg(columnIndx);
    }
    sqlQuery.exec(strQuery);
    while (sqlQuery.next())
        m_id.push_back(sqlQuery.value(0).toInt());
    sqlQuery.clear();
}

QList<QStringList> DBExchange::getInfo(int idDet)
{
    QString strQuery1;
    QSqlQuery sqlQuery1;
    QString strQuery2;
    QSqlQuery sqlQuery2;
    QString res;
    QStringList data;
    QList<QStringList> modelProto;

    strQuery1 = QString("SELECT Color, BoxIndex, RowIndex, ColumnIndex, Count, id FROM Placement WHERE Detail = '%1'").arg(m_id.at(idDet-1));
    sqlQuery1.exec(strQuery1);
        while (sqlQuery1.next())
        {
            data.append(sqlQuery1.value(0).toString());
            data.append(sqlQuery1.value(1).toString());
            data.append(sqlQuery1.value(2).toString());
            data.append(sqlQuery1.value(3).toString());
            data.append(sqlQuery1.value(4).toString());
            data.append(sqlQuery1.value(5).toString());
            res = data.at(0);
            int index;
            index = res.toInt();
            strQuery2 = QString("SELECT ColorValue FROM Color WHERE id = %1").arg(index);
            sqlQuery2.exec(strQuery2);
            sqlQuery2.next();
            data.replace(0, sqlQuery2.value(0).toString());
            modelProto.append(data);
            data.clear();
        }
    sqlQuery1.clear();
    sqlQuery2.clear();
    return modelProto;
}
