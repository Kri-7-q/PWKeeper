TEMPLATE = app

QT += qml quick widgets sql

SOURCES += main.cpp \
    tablemodel.cpp \
    persistence.cpp \
    account.cpp \
    sqlexception.cpp \
    listviewcontroler.cpp \
    viewcontroler.cpp \
    modifycontroler.cpp \
    pwgenerator.cpp \
    characterdefinition.cpp \
    showcontroler.cpp

RESOURCES += qml.qrc \
    pictures.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    tablemodel.h \
    persistence.h \
    account.h \
    sqlexception.h \
    listviewcontroler.h \
    viewcontroler.h \
    modifycontroler.h \
    pwgenerator.h \
    characterdefinition.h \
    showcontroler.h

DISTFILES +=
