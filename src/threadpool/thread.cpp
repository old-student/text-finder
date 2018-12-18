#include "thread.h"
#include "worker.h"

namespace scan {

struct Thread::Impl
{
    Impl(Thread& self)
        : self(self)
        , worker(new Worker())
    {
        worker->moveToThread(&self);
        QObject::connect(&self, &Thread::processUrl, worker, &Worker::processUrl);
        QObject::connect(worker, &Worker::urlFound, &self, &Thread::urlFound);
    }

    ~Impl()
    {
        delete worker;
    }

    // data members
    Thread& self;
    Worker* worker;
};

Thread::Thread(QObject* parent)
    : QThread(parent)
    , impl(new Impl(*this))
{
    start();
}

Thread::~Thread()
{
    quit();
    wait();
}

void Thread::run()
{
    QThread::run();
    impl.reset(nullptr);
}


}//namespace scan
