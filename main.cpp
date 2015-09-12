#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "controler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<TableModel>("Models", 1, 0, "TableModel");
    qmlRegisterType<Controler>("Controlers", 1, 0, "PWKeeperControler");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
