#include "appcore.h"
#include <QtWidgets>

DataBaseImageProvider::~DataBaseImageProvider()
{
  m_core = nullptr;
}

void DataBaseImageProvider::setCore(Appcore *core)
{
  m_core = core;
}

QPixmap DataBaseImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
  QImage img;
  QByteArray str;
  QString path;
  QPixmap pmap;

  if (id.right(2) == "/0")//если в конце пути изображения /0, значит требуется картинка-заглушка
  {
      img.load("assets//clr_iconPic.png");
      pmap = pmap.fromImage(img);
      return pmap;
  }

  if (id.contains("temp"))//если загружаем новую иконку
  {
      path = id.section("temp", 1);
      path.remove(0,8);
      img.load(path);
      pmap = pmap.fromImage(img);
      return pmap;
  }

  if (id.contains("dbSubCategory"))//если требуется картинка подкатегории
  {
      int picID;
      picID = id.section('/', 1).toInt();
      str = m_core->subCategoryEditExchange.getIcon(picID);
      pmap.loadFromData(str);
      return pmap;
  }

  if (id.contains("dbDetail"))//если требуется картинка детали в диалоге
  {
      int picID;
      picID = id.section('/', 1).toInt();
      str = m_core->detailEditExchange.getIcon(picID);
      pmap.loadFromData(str);
      return pmap;
  }
  if (id.contains("dbGetDetail"))//если требуется картинка для детали на основном окне
  {
      int picID;
      picID = id.section('/', 1).toInt();
      str = m_core->detailGridExchange.getIcon(picID);
      pmap.loadFromData(str);
      return pmap;
  }
  if (id.contains("dbFiltCat"))//если требуется картинка для детали на основном окне
  {
      int picID;
      picID = id.section('/', 1).toInt();
      str = m_core->detailInPlacementEditExchange.getIcon(picID);
      pmap.loadFromData(str);
      return pmap;
  }
  if (pmap.isNull())//если запросили картинку, которой нет
      m_core->sendToQML(QString("Returned null pxmap for: %1").arg(id), critical);
  return pmap;
}

Appcore::Appcore(QObject *parent)
{
  //сразу заполняем роли, по названию которых будем обращаться к данным в модели
  QStringList roles;
  roles.append("colorrole");
  roles.append("boxindex");
  roles.append("rowindex");
  roles.append("columnindex");
  roles.append("count");
  roles.append("idindex");
  m_DetailInfoModel.setRoles(roles);
}

Appcore::~Appcore()
{
  qmlEngine_ = nullptr;
  qmlContext_ = nullptr;
}

void Appcore::sendToQML(QString newText, int typeOfMsg)
{
  if (typeOfMsg == status)
      emit changeStatusBar(newText);
  else
      emit showMessage(newText, typeOfMsg);
}


void Appcore::openDatabase(QUrl nameOfDB)
{
  database_ = QSqlDatabase::addDatabase("QSQLITE");
  database_.setDatabaseName(nameOfDB.path());
  if (!database_.open())
  {
    QSqlError err;
    err = database_.lastError();
    sendToQML(QString("Can`t open DB. Error: %1").arg(err.text()), critical);
  }
  else
    sendToQML("Ьаза данных готова");
  initDBExchange();
  setContext();
  getDetails();

  emit showDetails();
}

void Appcore::closeDatabase()
{
    database_.removeDatabase(database_.connectionName());
    database_.close();
}

void Appcore::setEngine(QQmlApplicationEngine *engine)
{
    qmlEngine_ = engine;
    qmlContext_ = engine->rootContext();
}

void Appcore::initDBExchange()
{   
    categoryEditExchange.tableModel = new QSqlTableModel(this, database_);
    colourFamilyEditExchange.tableModel = new QSqlTableModel(this, database_);
    colourEditExchange.tableModel = new QSqlTableModel(this, database_);
    detailEditExchange.tableModel = new QSqlTableModel(this, database_);
    detailInPlacementEditExchange.tableModel = new QSqlTableModel(this, database_);
    detailGridExchange.tableModel = new QSqlTableModel(this, database_);
    placementEditExchange.tableModel = new QSqlTableModel(this, database_);
    subCategoryEditExchange.tableModel = new QSqlTableModel(this, database_);
    detailPlacementGridExchange.tableModel = new QSqlTableModel(this, database_);

    categoryEditExchange.setName("Category");
    colourFamilyEditExchange.setName("ColorFamily");
    colourEditExchange.setName("Color");
    detailEditExchange.setName("Detail");
    detailInPlacementEditExchange.setName("Detail");
    detailGridExchange.setName("Detail");
    placementEditExchange.setName("Placement");
    subCategoryEditExchange.setName("SubCategory");
    detailPlacementGridExchange.setName("Placement");

    categoryEditExchange.hasPicture = false;
    colourFamilyEditExchange.hasPicture = false;
    colourEditExchange.hasPicture = false;;
    detailEditExchange.hasPicture = true;
    detailInPlacementEditExchange.hasPicture = true;
    detailGridExchange.hasPicture = true;
    placementEditExchange.hasPicture = false;
    subCategoryEditExchange.hasPicture = true;
    detailPlacementGridExchange.hasPicture = false;

    categoryEditExchange.setDatabaseConnection(database_, "CategoryConnection");
    colourFamilyEditExchange.setDatabaseConnection(database_, "ColorFamilyConnection");
    colourEditExchange.setDatabaseConnection(database_, "ColorConnection");
    detailEditExchange.setDatabaseConnection(database_, "DetailConnection");
    detailInPlacementEditExchange.setDatabaseConnection(database_, "DetailPlacementConnection");
    detailGridExchange.setDatabaseConnection(database_, "DetailGridConnection");
    detailPlacementGridExchange.setDatabaseConnection(database_, "DetailPlacementGridConnection");
    placementEditExchange.setDatabaseConnection(database_, "PlacementConnection");
    subCategoryEditExchange.setDatabaseConnection(database_, "SubCategoryConnection");
    //subCategoryEditExchange.Eve(database_);
}

void Appcore::setContext()
{
  qmlContext_->setContextProperty("appExchangeCategoryEdit", &categoryEditExchange);
  qmlContext_->setContextProperty("appExchangeSubCategoryEdit", &subCategoryEditExchange);
  qmlContext_->setContextProperty("appExchangeColourFamilyEdit", &colourFamilyEditExchange);
  qmlContext_->setContextProperty("appExchangeColourEdit", &colourEditExchange);
  qmlContext_->setContextProperty("appExchangeDetailEdit", &detailEditExchange);
  qmlContext_->setContextProperty("appExchangePlacementEdit", &placementEditExchange);
  qmlContext_->setContextProperty("appExchangeDetailGrid", &detailGridExchange);
  qmlContext_->setContextProperty("appExchangeDetailInPlacementEdit", &detailInPlacementEditExchange);
}

void Appcore::openSubCategoryDialog()
{
    if (database_.isOpen())
    {
        QStringList categoryList;

        categoryList = categoryEditExchange.getName();
        m_CategoryModel.setStringList(categoryList);
        qmlContext_->setContextProperty("categoryModel", &m_CategoryModel);
        subCategoryEditExchange.getData();
    }

    emit showSubCategoryDialog();
}

void Appcore::openColorDialog()
{
    if (database_.isOpen())
    {
        //заполнение комбобокса
        QStringList list;
        list = colourFamilyEditExchange.getName();
        m_ColorFamilyModel.setStringList(list);
        qmlContext_->setContextProperty("colorFamilyModel", &m_ColorFamilyModel);
        //получение остальных данных для формы
        colourEditExchange.getData();
    }

    emit showColorDialog();
}

void Appcore::openDetailDialog(int index)
{
    if (database_.isOpen())
    {
        QStringList list;
        list = subCategoryEditExchange.getIDs();
        m_ShowSubCatModel.setStringList(list);
        qmlContext_->setContextProperty("subCatModel", &m_ShowSubCatModel);
        if (index == 0)
        {
          detailEditExchange.getData();
        }
        else
        {
          detailEditExchange.jumpID(index);
        }
    }

    emit showDetailDialog();
}

void Appcore::openPlacementDialog(int index, bool rawIndex)
{
    if (database_.isOpen())
    {
        /*QStringList list;
        list = colourEditExchange.getColorsList();
        m_ShowColorModel.setStringList(list);
        qmlContext_->setContextProperty("colorModel", &m_ShowColorModel);
        QStringList listId;
        listId = colourEditExchange.getColorsIdList();
        m_ShowColorIdModel.setStringList(list);*/
      QStringList listData;
      listData = colourEditExchange.getColorsIdList();
      m_ShowColorIdModel.setStringList(listData);
      qmlContext_->setContextProperty("colorModel", &m_ShowColorIdModel);
      //  qmlContext_->setContextProperty("colorIdModel", &m_ShowColorIdModel);

        if (index == 0)
            placementEditExchange.getData();
        else
        {
          if (rawIndex)
            placementEditExchange.jumpID(index);
          else
            placementEditExchange.jumpItem(index);
        }
    }

    emit showPlacementDialog();
}

void Appcore::openChooseDetailDialog(int catFilter) //Выбор детали в Диалоге Расположение
{
    int categoryFilter;
    if (database_.isOpen())
    {     
        QStringList list;
        categoryFilter = catFilter;
        detailInPlacementEditExchange.applyFilter(categoryFilter, 0, "0/0/0", list);
        m_DetailModel.setStringList(list);
        qmlContext_->setContextProperty("detailModel", &m_DetailModel);

        QStringList list1;
        //for (int i = 1; i <= subCategoryEditExchange.getSize(); i++)
        //    list1.append(QString("%1").arg(i));
        list1 = subCategoryEditExchange.getIDs();
        m_CategoryFilterInDetail.setStringList(list1);
        qmlContext_->setContextProperty("addCatFilterModel", &m_CategoryFilterInDetail);
    }

    emit showChooseDetailDialog(categoryFilter);

    QString str;
    int num;
    num = qrand();
    str.append(QString("%1").arg(num));
    emit refreshDetails4Placement(str);

    /*   if (m_dbLEGO.isOpen())
    {
        QStringList list;
        for (int i = 1; i <= m_Detail.getCount(); i++)
            list.append(QString("%1").arg(i));
        m_DetailModel.setStringList(list);
        m_Context->setContextProperty("detailModel", &m_DetailModel);
    }

    emit showChooseDetailDialog();*/
}

void Appcore::openChooseCatFilterDialog()
{
    if (database_.isOpen())
    {
        QStringList list;
        for (int i = 1; i <= subCategoryEditExchange.getSize(); i++)
            list.append(QString("%1").arg(i));
        m_ChooseCatModel.setStringList(list);
        qmlContext_->setContextProperty("catModel", &m_ChooseCatModel);
    }

    emit showChooseCatFilterDialog();
}

void Appcore::openChooseColorFilterDialog()
{
    if (database_.isOpen())
    {
        QStringList list, list1;
        QString temp;
        for (int i = 1; i <= colourEditExchange.getSize(); i++)
        {
            list = colourEditExchange.getColorsIdList();
           /* temp = colourTable.getFamilyColor(i);
            switch (temp) {
                case "1": temp = "T"; break;
                case "2": temp = "M"; break;
            default: temp = ""; break;
            }
            list1.append(temp);*/
        }
        m_ChooseColorModel.setStringList(list);
        qmlContext_->setContextProperty("colorFilterModel", &m_ChooseColorModel);
    }

    emit showChooseColorFilterDialog();
}

void Appcore::getDetails(int catFilter, int colorFilter, QString placeFilter)
{
  if (database_.isOpen())
  {
    QStringList list;
    detailGridExchange.applyFilter(catFilter, colorFilter, placeFilter, list);
    m_ShowDetailsModel.setStringList(list);
    qmlContext_->setContextProperty("showDetailsModel", &m_ShowDetailsModel);
  }

  QString str;
  int num;
  num = qrand();
  str.append(QString("%1").arg(num));
  emit setName(str);
}

void Appcore::openDetailInfo(int detailID)
{
  QList<QStringList> listData;
  m_DetailInfoModel.removeData();
  listData = detailPlacementGridExchange.getInfo(detailID);
  for (int i = 0; i < listData.size(); i++)
    m_DetailInfoModel.addData(listData.at(i));
  qmlContext_->setContextProperty("detailInfoModel", &m_DetailInfoModel);

  emit showDetailInfo(detailID);
}


