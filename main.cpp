#include <QApplication>
#include <QtQml>
#include "listviewcontroler.h"
#include "viewcontroler.h"
#include "modifycontroler.h"
#include "showcontroler.h"
#include "addcontroler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<TableModel>("Models", 1, 0, "TableModel");
    qmlRegisterType<ListViewControler>("Controllers", 1, 0, "ListViewController");
    qmlRegisterType<ViewControler>("Controllers", 1, 0, "ViewController");
    qmlRegisterType<ModifyControler>("Controllers", 1, 0, "ModifyController");
    qmlRegisterType<ShowControler>("Controllers", 1, 0, "ShowController");
    qmlRegisterType<AddControler>("Controllers", 1, 0, "AddController");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
