TEMPLATE = app

QT += qml quick widgets \
sql widgets \
widgets

QMAKE_CXXFLAGS += -std=c++11


SOURCES += main.cpp \
    appcore.cpp \
    dbexchange.cpp \
    dbexchangemodel.cpp

RESOURCES += qml.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    appcore.h \
    dbexchange.h \
    dbexchangemodel.h

DISTFILES += \
    FiltersDock.qml \
    application.qrc.txt
