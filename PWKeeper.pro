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
    Persistence/persistence.cpp \
    Persistence/credentials.cpp \
    addcontroler.cpp \
    insertationcontroller.cpp \
    controller.cpp
win: {
SOURCES += SqlDriver/PostgreSql/qsql_psql.cpp
}

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
    Persistence/persistence.h \
    Persistence/credentials.h \
    addcontroler.h \
    insertationcontroller.h \
    controller.h
win: {
HEADERS += \
    SqlDriver/PostgreSql/qobject_p.h \
    SqlDriver/PostgreSql/qsql_psql_p.h \
    SqlDriver/PostgreSql/qsqldriver_p.h \
    SqlDriver/PostgreSql/qsqlresult_p.h
}

DISTFILES +=
