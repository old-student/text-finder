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
