#include "thread.h"
#include "worker.h"

namespace scan {

Thread::Thread(QObject* parent)
    : QThread(parent)
    , worker(new Worker())
{
    worker->moveToThread(this);
    start();
}

Thread::~Thread()
{
    resume();
    stop();
    quit();
    wait();
}

void Thread::run()
{
    QScopedPointer<Worker> sp(worker);
    QThread::run();
}

void Thread::suspend()
{
    worker->suspend();
}

void Thread::resume()
{
    worker->resume();
}

void Thread::stop()
{
    worker->stop();
}

}//namespace scan
