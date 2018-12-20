#ifndef __GOSWCYGBEKZWNBYDPXGIQWADGASMSAFLHZHJCXKA__
#define __GOSWCYGBEKZWNBYDPXGIQWADGASMSAFLHZHJCXKA__

#include "types.h"
#include <QObject>
#include <memory>

namespace scan {

class Worker : public QObject
{
    Q_OBJECT

public:
    explicit Worker(QObject* parent = nullptr);
    ~Worker();

public:
    void suspend();
    void resume();

signals:
    void finished();

private slots:
    void suspendImpl();

signals:
    void urlFound(QUrl url);

public slots:
    void processRequest(Request request);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __GOSWCYGBEKZWNBYDPXGIQWADGASMSAFLHZHJCXKA__
