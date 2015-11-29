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
    Persistence/credentials.cpp \
    addcontroler.cpp \
    insertationcontroller.cpp \
    controller.cpp \
    SqlDriver/PostgreSql/kqpostgresqldriver.cpp \
    SqlDriver/PostgreSql/kqpostgresqlresult.cpp \
    Persistence/psqldatabase.cpp \
    Persistence/persistence.cpp

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
    Persistence/credentials.h \
    addcontroler.h \
    insertationcontroller.h \
    controller.h \
    SqlDriver/PostgreSql/kqpostgresqldriver.h \
    SqlDriver/PostgreSql/kqpostgresqlresult.h \
    Persistence/psqldatabase.h \
    Persistence/persistence.h

DISTFILES +=

macx: {
LIBS += -L/Application/PostgreSQL/9.4/lib/ -lpq

INCLUDEPATH += /Application/PostgreSQL/9.4/include
DEPENDPATH += /Application/PostgreSQL/9.4/include

PRE_TARGETDEPS += /Application/PostgreSQL/9.4/lib/libpq.a
}
