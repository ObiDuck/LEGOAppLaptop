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
    m_Engine = nullptr;
    m_Context = nullptr;
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
    //Указываем формат базы для объекта работы с ней
    m_dbLEGO = QSqlDatabase::addDatabase("QSQLITE");
    //Указываем какую базу открыть
    m_dbLEGO.setDatabaseName(nameOfDB.path());
    //Пробуем открыть базу
    if (!m_dbLEGO.open())
    {
        //Если не удалось открыть, выводим сообщение об ошибке
        QSqlError err;
        err = m_dbLEGO.lastError();
        //Выводим что не можем открыть базу и путь по которому пытаемся открыть
        sendToQML(QString("Can`t open DB. Error: %1").arg(err.text()), critical);
    }
    else
        sendToQML("Ьаза данных готова");//("Database is ready");

    //Создаем объекты для обмена данными с БД
    initDBExchange();
    setContext();
    getDetails();

    emit showDetails();
}

void Appcore::closeDatabase()
{
    m_dbLEGO.removeDatabase(m_dbLEGO.connectionName());
    m_dbLEGO.close();
}

void Appcore::setEngine(QQmlApplicationEngine *engine)
{
    m_Engine = engine;
    m_Context = engine->rootContext();
}

void Appcore::initDBExchange()
{   
    //заполняем названия столбоцов в таблицах
    QStringList tableItems;

    tableItems.append("Name");
    categoryEditExchange.setTable("Category", tableItems);
    colourFamilyEditExchange.setTable("ColorFamily", tableItems);
    tableItems.clear();

    tableItems.append("Name");
    tableItems.append("Family");
    tableItems.append("ColorValue");
    colourEditExchange.setTable("Color", tableItems);
    tableItems.clear();

    tableItems.append("Name");
    tableItems.append("SubCategory");
    tableItems.append("Picture");
    detailEditExchange.setTable("Detail", tableItems);
    detailInPlacementEditExchange.setTable("Detail", tableItems);
    detailGridExchange.setTable("Detail", tableItems);
    tableItems.clear();

    tableItems.append("Detail");
    tableItems.append("Color");
    tableItems.append("Count");
    tableItems.append("BoxIndex");
    tableItems.append("RowIndex");
    tableItems.append("ColumnIndex");
    placementEditExchange.setTable("Placement", tableItems);
    tableItems.clear();

    tableItems.append("Name");
    tableItems.append("Category");
    tableItems.append("Picture");
    subCategoryEditExchange.setTable("SubCategory", tableItems);
    tableItems.clear();
}

void Appcore::setContext()
{
    m_Context->setContextProperty("appExchangeCategoryEdit", &categoryEditExchange);
    m_Context->setContextProperty("appExchangeSubCategoryEdit", &subCategoryEditExchange);
    m_Context->setContextProperty("appExchangeColourFamilyEdit", &colourFamilyEditExchange);
    m_Context->setContextProperty("appExchangeColourEdit", &colourEditExchange);
    m_Context->setContextProperty("appExchangeDetailEdit", &detailEditExchange);
    m_Context->setContextProperty("appExchangePlacementEdit", &placementEditExchange);
    m_Context->setContextProperty("appExchangeDetailGrid", &detailGridExchange);
    m_Context->setContextProperty("appExchangeDetailInPlacementEdit", &detailInPlacementEditExchange);
}

void Appcore::openSubCategoryDialog()
{
    if (m_dbLEGO.isOpen())
    {
        QStringList categoryList;
        categoryList = categoryEditExchange.getName();
        m_CategoryModel.setStringList(categoryList);
        m_Context->setContextProperty("categoryModel", &m_CategoryModel);
        subCategoryEditExchange.getData();
    }

    emit showSubCategoryDialog();
}

void Appcore::openColorDialog()
{
    if (m_dbLEGO.isOpen())
    {
        //заполнение комбобокса
        QStringList list;
        list = colourFamilyEditExchange.getName();
        m_ColorFamilyModel.setStringList(list);
        m_Context->setContextProperty("colorFamilyModel", &m_ColorFamilyModel);
        //получение остальных данных для формы
        colourEditExchange.getData();
    }

    emit showColorDialog();
}

void Appcore::openDetailDialog()
{
    if (m_dbLEGO.isOpen())
    {
        QStringList list;
        for (int i = 1; i <= subCategoryEditExchange.getCount(); i++)
            list.append(QString("%1").arg(i));
        m_ShowSubCatModel.setStringList(list);
        m_Context->setContextProperty("subCatModel", &m_ShowSubCatModel);
        detailEditExchange.getData();
    }

    emit showDetailDialog();
}

void Appcore::openPlacementDialog(int index)
{
    if (m_dbLEGO.isOpen())
    {
        QStringList list;
        for (int i = 1; i <= colourEditExchange.getCount(); i++)
            list.append(colourEditExchange.getColor(i));
        m_ShowColorModel.setStringList(list);
        m_Context->setContextProperty("colorModel", &m_ShowColorModel);
        if (index == 0)
            placementEditExchange.getData();
        else
            placementEditExchange.jumpItem(index);
    }

    emit showPlacementDialog();
}

void Appcore::openChooseDetailDialog(int catFilter) //Выбор детали в Диалоге Расположение
{
    int categoryFilter;
    if (m_dbLEGO.isOpen())
    {     
        categoryFilter = subCategoryEditExchange.getID(catFilter);
        detailInPlacementEditExchange.applyFilter(categoryFilter, 0, "0/0/0");
        QStringList list;
        for (int i = 1; i <= detailInPlacementEditExchange.getCount(); i++)
            list.append(QString("%1").arg(i));
        m_DetailModel.setStringList(list);
        m_Context->setContextProperty("detailModel", &m_DetailModel);

        QStringList list1;
        for (int i = 1; i <= subCategoryEditExchange.getCount(); i++)
            list1.append(QString("%1").arg(i));
        m_CategoryFilterInDetail.setStringList(list1);
        m_Context->setContextProperty("addCatFilterModel", &m_CategoryFilterInDetail);
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
    if (m_dbLEGO.isOpen())
    {
        QStringList list;
        for (int i = 1; i <= subCategoryEditExchange.getCount(); i++)
            list.append(QString("%1").arg(i));
        m_ChooseCatModel.setStringList(list);
        m_Context->setContextProperty("catModel", &m_ChooseCatModel);
    }

    emit showChooseCatFilterDialog();
}

void Appcore::openChooseColorFilterDialog()
{
    if (m_dbLEGO.isOpen())
    {
        QStringList list, list1;
        QString temp;
        for (int i = 1; i <= colourEditExchange.getCount(); i++)
        {
            list.append(colourEditExchange.getColor(i));
           /* temp = colourTable.getFamilyColor(i);
            switch (temp) {
                case "1": temp = "T"; break;
                case "2": temp = "M"; break;
            default: temp = ""; break;
            }
            list1.append(temp);*/
        }
        m_ChooseColorModel.setStringList(list);
        m_Context->setContextProperty("colorFilterModel", &m_ChooseColorModel);
    }

    emit showChooseColorFilterDialog();
}

void Appcore::getDetails(int catFilter, int colorFilter, QString placeFilter)
{
    if (m_dbLEGO.isOpen())
    {
        int categoryFilter, colFilter;
        categoryFilter = subCategoryEditExchange.getID(catFilter);
        colFilter = colourEditExchange.getID(colorFilter);
        detailGridExchange.applyFilter(categoryFilter, colFilter, placeFilter);
        QStringList list;
        for (int i = 1; i <= detailGridExchange.getCount(); i++)
            list.append(QString("%1").arg(i));
        m_ShowDetailsModel.setStringList(list);
        m_Context->setContextProperty("showDetailsModel", &m_ShowDetailsModel);
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
    listData = detailGridExchange.getInfo(detailID);
    for (int i = 0; i < listData.size(); i++)
    {
        m_DetailInfoModel.addData(listData.at(i));
    }

    m_Context->setContextProperty("detailInfoModel", &m_DetailInfoModel);

    emit showDetailInfo();
}


