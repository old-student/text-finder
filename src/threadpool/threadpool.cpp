#include "threadpool.h"
#include "thread.h"
#include "worker.h"
#include "report.h"
#include <QVector>
#include <QQueue>

namespace scan {

struct ThreadPool::Impl
{
    Impl(ThreadPool& self)
        : self(self)
        , reportModel(nullptr)
        , requestLimit(0)
        , requestedCount(0)
        , finishedRequestCount(0)
        , isSuspended(false)
    {}

    ~Impl()
    {
        stop();
        cleanup();
    }

    void cleanup()
    {
        for (Thread* thread : threads) {
            delete thread;
        }
        threads.clear();
        availableWorkers.clear();
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
        availableWorkers.enqueue(thread->worker);
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
        for (Thread* thread : threads) {
            thread->stop();
        }
        cleanup();
    }

    void increaseProgress()
    {
        ++finishedRequestCount;
        emit self.progressValueChanged();
    }

    void processUrl(QUrl url)
    {
        if (isSuspended || availableWorkers.isEmpty()) {
            pendingRequests.enqueue(url);
            return;
        }

        Request request = reportModel ? Request(url, searchText,
                                                reportModel->registerRequest(url))
                                      : Request(url, searchText);
        QMetaObject::invokeMethod(availableWorkers.dequeue(),
                                  "processRequest",
                                  Q_ARG(Request, request));
    }

    // data members
    ThreadPool& self;
    ReportModel* reportModel;
    QVector<Thread*> threads;
    QQueue<Worker*> availableWorkers;
    QQueue<QUrl> pendingRequests;
    QString searchText;
    size_t requestLimit;
    size_t requestedCount;
    size_t finishedRequestCount;
    bool isSuspended;
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

void ThreadPool::initialize(const size_t threadCount,
                            const size_t requestLimit,
                            const QString& searchText)
{
    impl->isSuspended = false;
    impl->setThreadCount(threadCount);
    impl->searchText = searchText;
    impl->requestLimit = requestLimit;
    impl->requestedCount = 0;
    impl->finishedRequestCount = 0;
}

void ThreadPool::suspend()
{
    impl->isSuspended = true;
    impl->suspend();
}

void ThreadPool::resume()
{
    impl->resume();
    impl->isSuspended = false;
}

void ThreadPool::stop()
{
    impl->isSuspended = true;
    impl->stop();
}

float ThreadPool::getProgressValue() const
{
    if (impl->requestLimit == 0) {
        return 0.0f;
    } else if (impl->finishedRequestCount == impl->requestedCount) {
        return 1.0f;
    }
    return static_cast<float>(impl->finishedRequestCount) / impl->requestLimit;
}

void ThreadPool::processUrl(QUrl url)
{
    if (impl->threads.size() == 0) { return; }
    if (impl->requestedCount == impl->requestLimit) { return; }
    ++impl->requestedCount;
    impl->processUrl(url);
}

void ThreadPool::requestFinished(Worker* worker)
{
    impl->increaseProgress();

    impl->availableWorkers.enqueue(worker);

    const auto n = qMin(impl->pendingRequests.size(), impl->availableWorkers.size());
    for (int i = 0; i < n; ++i) {
        impl->processUrl(impl->pendingRequests.dequeue());
    }

    if (impl->finishedRequestCount == impl->requestedCount) {
        stop();
        emit finished();
    }
}

}//namespace scan
