#include <QApplication>
#include <QtQml>
#include "listviewcontroler.h"
#include "viewcontroler.h"
#include "modifycontroler.h"
#include "showcontroler.h"
#include "Persistence/credentials.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<TableModel>("Models", 1, 0, "TableModel");
    qmlRegisterType<ListViewControler>("Controlers", 1, 0, "ListViewControler");
    qmlRegisterType<ViewControler>("Controlers", 1, 0, "ViewControler");
    qmlRegisterType<ModifyControler>("Controlers", 1, 0, "ModifyControler");
    qmlRegisterType<ShowControler>("Controlers", 1, 0, "ShowControler");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
