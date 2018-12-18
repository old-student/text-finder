#include "qmlproxy/scanner.h"
#include <QGuiApplication>
#include <QtQml>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<scan::Scanner>("scanner", 1, 0, "Scanner", ""); 
    scan::Scanner scanner{};
    engine.rootContext()->setContextProperty("scanner", &scanner);    

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;           

    return app.exec();
}
