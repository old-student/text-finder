#include "worker.h"
#include <QGuiApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>
#include <QWaitCondition>

namespace scan {

struct Worker::Impl
{
    Impl(Worker& self)
        : self(self)
    {}

    void processRequest(Request request)
    {
        request.updater(Request::Status::Downloading);

        QNetworkAccessManager networkManager;
        QNetworkReply* reply = networkManager.get(QNetworkRequest(request.url));

        while (!reply->isFinished()) {
            qApp->processEvents();
        }

        if (reply->error()) {
            request.updater(Request::Status::Error);
            qDebug() << reply->errorString();
            reply->deleteLater();
            return;
        }

        const QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if (!redirectUrl.isEmpty()) {
            emit self.urlFound(redirectUrl);
            request.updater(Request::Status::Finished);
            reply->deleteLater();
            return;
        }

        request.updater(Request::Status::Processing);
        // TODO do parsing
        request.updater(Request::Status::Finished);
        reply->deleteLater();
    }

    // data members
    Worker& self;
    QMutex waitMutex;
    QWaitCondition waitCond;
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

void Worker::processRequest(Request request)
{
    impl->processRequest(request);
    emit finished();
}

}//namespace scan
