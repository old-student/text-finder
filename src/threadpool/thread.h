#ifndef __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__
#define __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__

#include <QThread>
#include <QUrl>
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

signals:
    void processUrl(const QUrl& url);
    void urlFound(const QUrl& url);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __NPQBJGPYHPTAMQTXCJDYQHVSCIMPRCKXQVTVKXBJ__
