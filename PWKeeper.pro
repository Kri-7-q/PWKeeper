TEMPLATE = app

QT += qml quick widgets sql

SOURCES += main.cpp \
    tablemodel.cpp \
    account.cpp \
    Exception/sqlexception.cpp \
    listviewcontroler.cpp \
    viewcontroler.cpp \
    modifycontroler.cpp \
    Generator/pwgenerator.cpp \
    Generator/characterdefinition.cpp \
    showcontroler.cpp \
    Persistence/persistence.cpp \
    Persistence/credentials.cpp

RESOURCES += QML/qml.qrc \
             QML/pictures.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    tablemodel.h \
    account.h \
    Exception/sqlexception.h \
    listviewcontroler.h \
    viewcontroler.h \
    modifycontroler.h \
    Generator/pwgenerator.h \
    Generator/characterdefinition.h \
    showcontroler.h \
    Persistence/persistence.h \
    Persistence/credentials.h

DISTFILES +=
