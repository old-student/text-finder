#include "scanner.h"

namespace scan {

Scanner::Scanner(QObject* parent)
    : QObject(parent)
    , reportModel(new ReportModel(this))
    , status(Status::NotRunning)
{
    pool.setReportModel(reportModel);
    QObject::connect(&pool, &ThreadPool::progressValueChanged,
                     this, &Scanner::progressValueChanged);
    QObject::connect(&pool, &ThreadPool::finished,
                     [this](){ setStatus(Status::Finished); });
}

Scanner::~Scanner() = default;

float Scanner::getProgressValue() const
{
    return pool.getProgressValue();
}

void Scanner::start()
{
    reportModel->clear();
    pool.initialize(threadCount, requestLimit, searchText);
    setStatus(Status::Running);
    pool.processUrl(QUrl(startUrl));
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
