#include "threadpool.h"
#include "thread.h"
#include "worker.h"
#include "report.h"
#include <QVector>
#include <QtDebug>

namespace scan {

struct ThreadPool::Impl
{
    Impl(ThreadPool& self)
        : self(self)
        , reportModel(nullptr)
        , requestLimit(0)
    {}

    ~Impl()
    {
        cleanup();
    }

    void cleanup()
    {
        for (Thread* thread : threads) {
            delete thread;
        }
        threads.clear();
    }

    void connect(Thread* t)
    {
        Worker* w = t->worker;
        QObject::connect(w, &Worker::urlFound, &self, &ThreadPool::processUrl);
        QObject::connect(w, &Worker::finished, &self, &ThreadPool::requestFinished);
    }

    void addThread()
    {
        Thread* thread = new Thread(&self);
        connect(thread);
        threads.push_back(thread);
    }

    void setThreadCount(const size_t n)
    {
        cleanup();
        for (size_t i = 0; i < n; ++i) {
            addThread();
        }
    }

    void suspend()
    {
        for (Thread* thread : threads) {
            thread->suspend();
        }
    }

    void resume()
    {
        for (Thread* thread : threads) {
            thread->resume();
        }
    }

    void stop()
    {
        cleanup();
    }

    void processUrl(QUrl url)
    {
        Request request = reportModel ? Request(url, reportModel->registerRequest(url))
                                      : Request(url);
        // TODO need load balancing
        Worker* w = threads[rand() % threads.size()]->worker;
        QMetaObject::invokeMethod(w, "processRequest", Q_ARG(Request, request));
    }

    // data members
    ThreadPool& self;
    ReportModel* reportModel;
    QVector<Thread*> threads;
    size_t requestLimit;
};

ThreadPool::ThreadPool(QObject *parent)
    : QObject(parent)
    , impl(new Impl(*this))
{}

ThreadPool::~ThreadPool() = default;

void ThreadPool::setReportModel(ReportModel* reportModel)
{
    impl->reportModel = reportModel;
}

void ThreadPool::init(const size_t threadCount, const size_t requestLimit)
{
    impl->setThreadCount(threadCount);
    impl->requestLimit = requestLimit;
}

void ThreadPool::suspend()
{
    impl->suspend();
}

void ThreadPool::resume()
{
    impl->resume();
}

void ThreadPool::stop()
{
    impl->stop();
}

void ThreadPool::processUrl(QUrl url)
{
    qDebug() << "[ThreadPool][processUrl]" << QThread::currentThread();
    impl->processUrl(url);
}

void ThreadPool::requestFinished()
{
    qDebug() << "[ThreadPool][requestFinished]";
}

}//namespace scan
