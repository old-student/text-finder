#ifndef __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__
#define __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__

#include <QThread>
#include <memory>

namespace scan {

class Thread : public QThread
{
    Q_OBJECT

public:
    explicit Thread(QObject* parent = nullptr);
    virtual ~Thread();

protected:
    void run() override;

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__
