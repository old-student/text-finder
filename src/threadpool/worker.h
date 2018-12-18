#ifndef __GOSWCYGBEKZWNBYDPXGIQWADGASMSAFLHZHJCXKA__
#define __GOSWCYGBEKZWNBYDPXGIQWADGASMSAFLHZHJCXKA__

#include <QObject>
#include <memory>

namespace scan {

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject* parent = nullptr);
    virtual ~Worker();

public:
    void suspend();
    void resume();

private slots:
    void suspendImpl();

signals:
    void urlFound(const QUrl& url);

public slots:
    void processUrl(const QUrl& url);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __GOSWCYGBEKZWNBYDPXGIQWADGASMSAFLHZHJCXKA__
