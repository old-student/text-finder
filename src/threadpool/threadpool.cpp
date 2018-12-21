#include "threadpool.h"
#include "thread.h"
#include "worker.h"
#include "report.h"
#include <QVector>
#include <QQueue>

namespace scan {

struct ThreadPool::Impl
{
    struct State
    {
        void reset()
        {
            isSuspended = false;
            requestCount = 0;
            finishedRequestCount = 0;
        }

        bool isDone() const
        {
            return requestCount == finishedRequestCount;
        }

        bool isSuspended = false;
        size_t requestCount = 0;
        size_t finishedRequestCount = 0;
    };

    Impl(ThreadPool& self)
        : self(self)
        , reportModel(nullptr)
        , requestLimit(0)
    {}

    ~Impl()
    {
        stop();
        cleanup();
    }

    void cleanup()
    {
        for (Thread* thread : threads) { delete thread; }
        threads.clear();
        availableWorkers.clear();
        pendingRequests.clear();
    }

    void addThread()
    {
        Thread* thread = new Thread(&self);
        QObject::connect(thread->worker, &Worker::urlFound, &self, &ThreadPool::processUrl);
        QObject::connect(thread->worker, &Worker::finished, &self, &ThreadPool::requestFinished);
        threads.push_back(thread);
        availableWorkers.enqueue(thread->worker);
    }

    void setThreadCount(const size_t n)
    {
        cleanup();
        for (size_t i = 0; i < n; ++i) { addThread(); }
    }

    void suspend()
    {
        state.isSuspended = true;
        for (Thread* thread : threads) { thread->suspend(); }
    }

    void resume()
    {
        for (Thread* thread : threads) { thread->resume(); }
        state.isSuspended = false;
    }

    void stop()
    {
        state.isSuspended = true;
        for (Thread* thread : threads) { thread->stop(); }
        cleanup();
    }

    float getProgressValue() const
    {
        if (requestLimit == 0) { return 0.0f; }
        if (state.isDone()) { return 1.0f; }
        return static_cast<float>(state.finishedRequestCount) / requestLimit;
    }

    bool isRequestLimitReached() const
    {
        return state.requestCount == requestLimit;
    }

    void increaseProgress()
    {
        ++state.finishedRequestCount;
        emit self.progressValueChanged();
    }

    void processUrl(QUrl url)
    {
        if ( state.isSuspended || availableWorkers.isEmpty()) {
            pendingRequests.enqueue(url);
            return;
        }
        Request request = reportModel ? Request(url, searchText, reportModel->registerRequest(url))
                                      : Request(url, searchText);
        QMetaObject::invokeMethod(availableWorkers.dequeue(),
                                  "processRequest",
                                  Q_ARG(Request, request));
    }

    void processPendingRequests()
    {
        const auto diff = qMin(pendingRequests.size(), availableWorkers.size());
        for (int i = 0; i < diff; ++i) {
            processUrl(pendingRequests.dequeue());
        }
    }

    // data members
    ThreadPool& self;
    ReportModel* reportModel;
    QVector<Thread*> threads;
    QQueue<Worker*> availableWorkers;
    QQueue<QUrl> pendingRequests;
    State state;
    QString searchText;
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

void ThreadPool::initialize(const size_t threadCount,
                            const size_t requestLimit,
                            const QString& searchText)
{
    impl->setThreadCount(threadCount);
    impl->requestLimit = requestLimit;
    impl->searchText = searchText;
    impl->state.reset();
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

float ThreadPool::getProgressValue() const
{
    return impl->getProgressValue();
}

void ThreadPool::processUrl(QUrl url)
{
    if (impl->threads.size() == 0) { return; }
    if (impl->isRequestLimitReached()) { return; }
    ++impl->state.requestCount;
    impl->processUrl(url);
}

void ThreadPool::requestFinished(Worker* worker)
{
    impl->increaseProgress();
    impl->availableWorkers.enqueue(worker);

    if (!impl->state.isDone()) {
        impl->processPendingRequests();
        return;
    }

    impl->stop();
    emit finished();
}

}//namespace scan
