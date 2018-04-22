#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include "appcore.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //создаем движок qml
    QQmlApplicationEngine *engine = new QQmlApplicationEngine;

    Appcore appCore;
    appCore.setEngine(engine);

    QQmlContext *context = engine->rootContext();;


    DataBaseImageProvider* imgProv = new DataBaseImageProvider;
    imgProv->setCore(&appCore);
    engine->addImageProvider(QLatin1String("imageProvider"), imgProv);
    engine->load(QUrl(QStringLiteral("qrc:/main.qml")));
    context->setContextProperty("appCore", &appCore);

    return app.exec();
}
