#include "scanner.h"
#include <QTimer>
#include <QtDebug>

namespace scan {

Scanner::Scanner(QObject* parent)
    : QObject(parent)
    , reportModel(new ReportModel(this))
    , status(Status::NotRunning)
{
    static QTimer t;
    QObject::connect(&t, &QTimer::timeout, [this](){
        reportModel->addEntry(QUrl("https://www.facebook.com"));
    });
    t.setInterval(1000);
    t.start();
}

Scanner::~Scanner() = default;

void Scanner::start()
{
    qDebug() << "start";
}

void Scanner::stop()
{
    qDebug() << "stop";
}

void Scanner::pause()
{
    qDebug() << "pause";
}

}// namespace scan
