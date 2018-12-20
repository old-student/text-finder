#include "scanner.h"
#include <QTimer>
#include <QtDebug>

namespace scan {

Scanner::Scanner(QObject* parent)
    : QObject(parent)
    , reportModel(new ReportModel(this))
    , status(Status::NotRunning)
{
    pool.setReportModel(reportModel);
}

Scanner::~Scanner() = default;

void Scanner::start()
{
    pool.setThreadCount(threadsNumber);
    pool.processUrl(QUrl(startUrl));
    setStatus(Status::Running);
}

void Scanner::suspend()
{
    pool.suspend();
    setStatus(Status::Suspended);
}

void Scanner::resume()
{
    pool.resume();
    setStatus(Status::Running);
}

void Scanner::stop()
{
    pool.stop();
    setStatus(Status::NotRunning);
}

}// namespace scan
