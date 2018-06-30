#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QtSql>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include "dbexchange.h"
#include "dbexchangemodel.h"
#include <QVector>
#include <QQuickImageProvider>

enum msgType {status = 0, info, warning, critical, question};

class Appcore : public QObject
{
    Q_OBJECT
private:
    QQmlApplicationEngine* qmlEngine_;
    QQmlContext* qmlContext_;//контекст qml
    QStringListModel m_CategoryModel;//модель имен категорий
    QStringListModel m_ColorFamilyModel;//модель имен семейств цвета
    QStringListModel m_DetailModel;//модель деталей для выбора в диалоговом окне
    QStringListModel m_ShowDetailsModel;//модель деталей для показа на основном экране
    QStringListModel m_ShowSubCatModel;//модель для показа подкатегорий в диалоге детали
    QStringListModel m_ShowColorModel;//модель для показа цвета в расположении
    QStringListModel m_ChooseCatModel;//модель для выбора фильтра подкатегорий
    QStringListModel m_ChooseColorModel;//модель для выбора фильтра цвета
    QStringListModel m_ColorFamilyModel2Choose;//модель для выбора фильтра цвета
    QStringListModel m_CategoryFilterInDetail;//модель для выбора фильтра цвета
    DBExchangeModel m_DetailInfoModel;//модель для вывода информации о детали
    QSqlDatabase database_;
public:
    explicit Appcore(QObject *parent = 0);
    DBExchange categoryEditExchange;
    DBExchange colourEditExchange;
    DBExchange colourFamilyEditExchange;
    DBExchange detailEditExchange;
    DBExchange m_Orientation;
    DBExchange placementEditExchange;
    DBExchange subCategoryEditExchange;
    DBExchange detailGridExchange;
    DBExchange detailPlacementGridExchange;
    DBExchange detailInPlacementEditExchange;
    ~Appcore();
    void setEngine(QQmlApplicationEngine *engine);
    void initDBExchange();
    void setContext();
signals:
    void changeStatusBar(QString newText);
    void showMessage(QString newText, int typeOfMsg);
    void showSubCategoryDialog();
    void showColorDialog();
    void showDetailDialog();
    void showPlacementDialog();
    void showChooseDetailDialog(int chosenFilter);
    void showChooseCatFilterDialog();
    void showChooseColorFilterDialog();
    void showDetails();
    void showDetailInfo();
    void setName(QString newText);
    void refreshDetails4Placement(QString newText);
public slots:
    void openSubCategoryDialog();
    void sendToQML(QString newText, int typeOfMsg = status);
    void openDatabase(QUrl nameOfDB);
    void closeDatabase();
    void openColorDialog();
    void openDetailDialog();
    void openPlacementDialog(int index = 0, bool rawIndex = false);
    void openChooseDetailDialog(int catFilter);
    void openChooseCatFilterDialog();
    void openChooseColorFilterDialog();
    void openDetailInfo(int detailID);
    void getDetails(int catFilter = 0, int colorFilter = 0, QString placeFilter = "0/0/0");
};

class DataBaseImageProvider : public QQuickImageProvider
{
public:
    DataBaseImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap) {}
    void setCore(Appcore *core);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    ~DataBaseImageProvider();
private:
    Appcore* m_core;
};
#endif // APPCORE_H
