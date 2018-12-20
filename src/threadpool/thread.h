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

protected:
    void run() override;

public:
    void suspend();
    void resume();

public:
    Worker* worker;
};

}//namespace scan

#endif // __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__
