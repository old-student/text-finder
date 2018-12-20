#ifndef __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__
#define __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__

#include <QObject>
#include <QUrl>
#include <memory>

namespace scan {

class ReportModel;

class ThreadPool : public QObject
{
    Q_OBJECT

public:
    explicit ThreadPool(QObject* parent = nullptr);
    ~ThreadPool();

public:
    void setReportModel(ReportModel* reportModel);
    void setThreadCount(const size_t n);
    void suspend();
    void resume();
    void stop();

public slots:
    void processUrl(QUrl url);
    void requestFinished();

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__
