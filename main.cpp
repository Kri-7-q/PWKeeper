#include <QGuiApplication>
#include <QtQml>
#include "tableviewmodel.h"
#include "listviewcontroler.h"
#include "viewcontroler.h"
#include "modifycontroler.h"
#include "addcontroler.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<TableViewModel>("Models", 1, 0, "TableViewModel");
    qmlRegisterType<ListViewControler>("Controllers", 1, 0, "ListViewController");
    qmlRegisterType<ViewControler>("Controllers", 1, 0, "ViewController");
    qmlRegisterType<ModifyControler>("Controllers", 1, 0, "ModifyController");
    qmlRegisterType<Controller>("Controllers", 1, 0, "DialogController");
    qmlRegisterType<AddControler>("Controllers", 1, 0, "AddController");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
