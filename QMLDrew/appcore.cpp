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
        str = m_core->m_SubCategory.getIcon(picID);
        pmap.loadFromData(str);
        return pmap;
    }

    if (id.contains("dbDetail"))//если требуется картинка детали в диалоге
    {
        int picID;
        picID = id.section('/', 1).toInt();
        str = m_core->m_Detail.getIcon(picID);
        pmap.loadFromData(str);
        return pmap;
    }
    if (id.contains("dbGetDetail"))//если требуется картинка для детали на основном окне
    {
        int picID;
        picID = id.section('/', 1).toInt();
        str = m_core->m_Detail2Show.getIcon(picID);
        pmap.loadFromData(str);
        return pmap;
    }
    if (id.contains("dbFiltCat"))//если требуется картинка для детали на основном окне
    {
        int picID;
        picID = id.section('/', 1).toInt();
        str = m_core->m_Detail4Placement.getIcon(picID);
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
    m_Category.setTable("Category", tableItems);
    m_ColorFamily.setTable("ColorFamily", tableItems);
    tableItems.clear();

    tableItems.append("Name");
    tableItems.append("Family");
    tableItems.append("ColorValue");
    m_Color.setTable("Color", tableItems);
    tableItems.clear();

    tableItems.append("Name");
    tableItems.append("SubCategory");
    tableItems.append("Picture");
    m_Detail.setTable("Detail", tableItems);
    m_Detail4Placement.setTable("Detail", tableItems);
    m_Detail2Show.setTable("Detail", tableItems);
    tableItems.clear();

    tableItems.append("Detail");
    tableItems.append("Color");
    tableItems.append("Count");
    tableItems.append("BoxIndex");
    tableItems.append("RowIndex");
    tableItems.append("ColumnIndex");
    m_Placement.setTable("Placement", tableItems);
    tableItems.clear();

    tableItems.append("Name");
    tableItems.append("Category");
    tableItems.append("Picture");
    m_SubCategory.setTable("SubCategory", tableItems);
    tableItems.clear();
}

void Appcore::setContext()
{
    m_Context->setContextProperty("dbCategory", &m_Category);
    m_Context->setContextProperty("dbSubCategory", &m_SubCategory);
    m_Context->setContextProperty("dbColorFamily", &m_ColorFamily);
    m_Context->setContextProperty("dbColor", &m_Color);
    m_Context->setContextProperty("dbDetail", &m_Detail);
    m_Context->setContextProperty("dbPlacement", &m_Placement);
    m_Context->setContextProperty("dbDetail2Show", &m_Detail2Show);
    m_Context->setContextProperty("dbDetail4Placement", &m_Detail4Placement);
}

void Appcore::openSubCategoryDialog()
{
    if (m_dbLEGO.isOpen())
    {
    //заполнение комбобокса для названий категорий
        QStringList list;
        list = m_Category.getName();
        m_CategoryModel.setStringList(list);
        m_Context->setContextProperty("categoryModel", &m_CategoryModel);
        //получение данных для остальных элементов
        m_SubCategory.getData();
    }

    emit showSubCategoryDialog();
}

void Appcore::openColorDialog()
{
    if (m_dbLEGO.isOpen())
    {
        //заполнение комбобокса
        QStringList list;
        list = m_ColorFamily.getName();
        m_ColorFamilyModel.setStringList(list);
        m_Context->setContextProperty("colorFamilyModel", &m_ColorFamilyModel);
        //получение остальных данных для формы
        m_Color.getData();
    }

    emit showColorDialog();
}

void Appcore::openDetailDialog()
{
    if (m_dbLEGO.isOpen())
    {
        QStringList list;
        for (int i = 1; i <= m_SubCategory.getCount(); i++)
            list.append(QString("%1").arg(i));
        m_ShowSubCatModel.setStringList(list);
        m_Context->setContextProperty("subCatModel", &m_ShowSubCatModel);
        m_Detail.getData();
    }

    emit showDetailDialog();
}

void Appcore::openPlacementDialog(int index)
{
    if (m_dbLEGO.isOpen())
    {
        QStringList list;
        for (int i = 1; i <= m_Color.getCount(); i++)
            list.append(m_Color.getColor(i));
        m_ShowColorModel.setStringList(list);
        m_Context->setContextProperty("colorModel", &m_ShowColorModel);
        if (index == 0)
            m_Placement.getData();
        else
            m_Placement.jumpItem(index);
    }

    emit showPlacementDialog();
}

void Appcore::openChooseDetailDialog(int catFilter) //Выбор детали в Диалоге Расположение
{
    int categoryFilter;
    if (m_dbLEGO.isOpen())
    {     
        categoryFilter = m_SubCategory.getID(catFilter);
        m_Detail4Placement.applyFilter(categoryFilter, 0, "0/0/0");
        QStringList list;
        for (int i = 1; i <= m_Detail4Placement.getCount(); i++)
            list.append(QString("%1").arg(i));
        m_DetailModel.setStringList(list);
        m_Context->setContextProperty("detailModel", &m_DetailModel);

        QStringList list1;
        for (int i = 1; i <= m_SubCategory.getCount(); i++)
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
        for (int i = 1; i <= m_SubCategory.getCount(); i++)
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
        for (int i = 1; i <= m_Color.getCount(); i++)
        {
            list.append(m_Color.getColor(i));
           /* temp = m_Color.getFamilyColor(i);
            switch (temp) {
                case "1": temp = "T"; break;
                case "2": temp = "M"; break;
            default: temp = ""; break;
            }
            list1.append(temp);*/
        }
        m_ChooseColorModel.setStringList(list);
        //m_ColorFamilyModel2Choose.setStringList(list1);
        m_Context->setContextProperty("colorFilterModel", &m_ChooseColorModel);
       // m_Context->setContextProperty("colorFamilyFilterModel", &m_ColorFamilyModel2Choose);
    }

    emit showChooseColorFilterDialog();
}

void Appcore::getDetails(int catFilter, int colorFilter, QString placeFilter)
{
    if (m_dbLEGO.isOpen())
    {
        int categoryFilter, colFilter;
        categoryFilter = m_SubCategory.getID(catFilter);
        colFilter = m_Color.getID(colorFilter);
        m_Detail2Show.applyFilter(categoryFilter, colFilter, placeFilter);
        QStringList list;
        for (int i = 1; i <= m_Detail2Show.getCount(); i++)
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
    listData = m_Detail2Show.getInfo(detailID);
    for (int i = 0; i < listData.size(); i++)
    {
        m_DetailInfoModel.addData(listData.at(i));
    }

    m_Context->setContextProperty("detailInfoModel", &m_DetailInfoModel);

    emit showDetailInfo();
}


