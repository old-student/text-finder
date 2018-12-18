//#include "scanner.h"
#include "threadpool/threadpool.h"
#include <QGuiApplication>
#include <QtQml>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    /*
    QQmlApplicationEngine engine;

    qmlRegisterUncreatableType<scan::Scanner>("scanner", 1, 0, "Scanner", "");

    scan::Scanner scanner{};
    engine.rootContext()->setContextProperty("scanner", &scanner);

    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
        */

    scan::ThreadPool threadPool;
    threadPool.setMaxThreadCount(5);
    for (int i = 0; i < 10; ++i) {
        threadPool.processUrl(QUrl("https://www.facebook.com"));
    }

    return app.exec();
}
