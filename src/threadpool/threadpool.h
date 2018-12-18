#ifndef __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__
#define __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__

#include <QObject>
#include <QUrl>
#include <memory>

namespace scan {

class ThreadPool : public QObject
{
    Q_OBJECT

public:
    explicit ThreadPool(QObject* parent = nullptr);
    virtual ~ThreadPool();

public:
    void setMaxThreadCount(const size_t n);

public slots:
    void processUrl(const QUrl& url);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __XCFUUPYUQIHZYNBTWVVGAQILCEKHGOHZOGSSQTKC__
