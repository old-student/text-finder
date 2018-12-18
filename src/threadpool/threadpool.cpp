#include "threadpool.h"
#include "thread.h"
#include <QVector>

namespace scan {

struct ThreadPool::Impl
{
    Impl(ThreadPool& self)
        : self(self)
    {
        addThread();
    }

    void clear()
    {
        for (Thread* thread : threads) {
            delete thread;
        }
        threads.clear();
    }

    void addThread()
    {
        Thread* thread = new Thread(&self);
        QObject::connect(thread, &Thread::urlFound, &self, &ThreadPool::processUrl);
        threads.push_back(thread);
    }

    void setMaxThreadCount(const size_t n)
    {
        clear();
        for (size_t i = 0; i < n; ++i) {
            addThread();
        }
    }

    void processUrl(const QUrl& url)
    {
        const auto i = rand() % threads.size();
        threads[i]->processUrl(url);
    }

    // data members
    ThreadPool& self;
    QVector<Thread*> threads;
};

ThreadPool::ThreadPool(QObject *parent)
    : QObject(parent)
    , impl(new Impl(*this))
{}

ThreadPool::~ThreadPool() = default;

void ThreadPool::setMaxThreadCount(const size_t n)
{
    impl->setMaxThreadCount(n);
}

void ThreadPool::processUrl(const QUrl &url)
{
    impl->processUrl(url);
}

}//namespace scan
