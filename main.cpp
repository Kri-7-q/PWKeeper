#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "controler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    TableModel model;
    Controler controler(&model);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("controler", &controler);
    engine.rootContext()->setContextProperty("tableModel", &model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
