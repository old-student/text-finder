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
    void init(const size_t threadCount, const size_t requestLimit);
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
