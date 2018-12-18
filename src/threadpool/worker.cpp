#include "worker.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>
#include <QWaitCondition>
#include <QtDebug>
#include <QThread>

namespace scan {

struct Worker::Impl
{
    Impl(Worker& self)
        : self(self)
        , networkManager(new QNetworkAccessManager(&self))
    {
        QObject::connect(networkManager, &QNetworkAccessManager::finished,
                         [this](QNetworkReply* reply) { handleReply(reply); });
    }

    void handleReply(QNetworkReply* reply)
    {
        QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if (redirectUrl.isEmpty()) {
            qDebug() << reply->readAll().size();
        } else {
            emit self.urlFound(redirectUrl);
        }
        reply->deleteLater();
    }

    void processUrl(const QUrl& url)
    {
        qDebug() << "Current thread" << QThread::currentThread();
        networkManager->get(QNetworkRequest(url));
    }

    // data members
    Worker& self;
    QMutex waitMutex;
    QWaitCondition waitCond;
    QNetworkAccessManager* networkManager;
};

Worker::Worker(QObject* parent)
    : QObject(parent)
    , impl(new Impl(*this))
{
    impl->waitMutex.lock();
}

Worker::~Worker()
{
    impl->waitCond.wakeAll();
    impl->waitMutex.unlock();
}

void Worker::suspend()
{
    QMetaObject::invokeMethod(this, "suspendImpl");
    // acquiring mutex to block the calling thread
    impl->waitMutex.lock();
    impl->waitMutex.unlock();
}

void Worker::suspendImpl()
{
    impl->waitCond.wait(&impl->waitMutex);
}

void Worker::resume()
{
    impl->waitCond.wakeAll();
}

void Worker::processUrl(const QUrl& url)
{
    impl->processUrl(url);
}

}//namespace scan
