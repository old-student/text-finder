#ifndef __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__
#define __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__

#include <QThread>
#include <QUrl>
#include <memory>

namespace scan {

class Worker;

class Thread : public QThread
{
public:
    explicit Thread(QObject* parent = nullptr);
    ~Thread();

public:
    void suspend();
    void resume();

public:
    Worker* worker;
};

}//namespace scan

#endif // __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__
