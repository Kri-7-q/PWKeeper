TEMPLATE = app

QT += qml quick widgets sql

SOURCES += main.cpp \
    tablemodel.cpp \
    controler.cpp \
    persistence.cpp \
    account.cpp \
    sqlexception.cpp

RESOURCES += qml.qrc \
    pictures.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    tablemodel.h \
    controler.h \
    persistence.h \
    account.h \
    sqlexception.h

DISTFILES +=
