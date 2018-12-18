#ifndef __TASK_H__
#define __TASK_H__

#include "threadpool/worker.h"
#include <QObject>
#include <QTimer>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QThread>
#include <QVector>
#include <QtDebug>
#include <QQueue>
#include <QMutex>
#include <QWaitCondition>

using namespace scan;

class Thread : public QThread
{
public:
    explicit Thread(QObject* parent = nullptr)
        : QThread(parent)
        , worker(new Worker())
    {
        worker->moveToThread(this);
        start();
    }

    ~Thread()
    {
        quit();
        wait();
    }

protected:
    void run() override
    {        
        QThread::run();
        delete worker;
    }

public:
    Worker* worker;
};

class ThreadPool : public QObject
{
    Q_OBJECT

public:
    explicit ThreadPool(QObject* parent = nullptr)
        : QObject(parent)
        , urlCount(0)
    {
        addThread();
        syncIdle();
    }

    virtual ~ThreadPool()
    {}

private:
    void addThread(int count = 1)
    {
        for (int i = 0; i < count; ++i) {
            Thread* thread = new Thread(this);
            threads.push_back(thread);
            QObject::connect(thread->worker, &Worker::urlFound, this, &ThreadPool::processUrl);
            /*
            QObject::connect(thread->worker, &Worker::idle, [this](Thread* thread){
                idleThreads.enqueue(thread);
            });
            */
        }
    }

    void syncIdle()
    {
        idleThreads.clear();
        for (Thread* t : threads) {
            idleThreads.enqueue(t);
        }
    }

public slots:
    void processUrl(const QUrl& url)
    {
        if (!idleThreads.isEmpty()) {
            Thread* t = idleThreads.dequeue();
            QMetaObject::invokeMethod(t->worker, "processUrl", Q_ARG(QUrl, url));
            return;
        }
        // might be not the best strategy
        const auto i = rand() % threads.size();
        QMetaObject::invokeMethod(threads.at(i)->worker, "processUrl", Q_ARG(QUrl, url));
    }

private:
    size_t urlCount;
    QVector<Thread*> threads;
    QQueue<Thread*> idleThreads;
};

#endif // __TASK_H__
