#include "thread.h"
#include "worker.h"

namespace scan {

Thread::Thread(QObject* parent)
    : QThread(parent)
    , worker(new Worker())
{
    worker->moveToThread(this);
    QObject::connect(this, &QThread::finished, worker, &Worker::deleteLater);
    start();
}

Thread::~Thread()
{
    resume();
    quit();
    wait();
}

void Thread::suspend()
{
    worker->suspend();
}

void Thread::resume()
{
    worker->resume();
}

}//namespace scan
