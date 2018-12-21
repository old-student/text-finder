#ifndef __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__
#define __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__

#include <QObject>
#include <QUrl>
#include <memory>

namespace scan {

class Worker;
class ReportModel;

class ThreadPool : public QObject
{
    Q_OBJECT

public:
    explicit ThreadPool(QObject* parent = nullptr);
    ~ThreadPool();

public:
    void setReportModel(ReportModel* reportModel);
    void initialize(const size_t threadCount,
                    const size_t requestLimit,
                    const QString& searchText);
    void suspend();
    void resume();
    void stop();
    float getProgressValue() const;

signals:
    void progressValueChanged();
    void finished();

public slots:
    void processUrl(QUrl url);
    void requestFinished(Worker* worker);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__
