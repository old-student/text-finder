#include "worker.h"
#include <QGuiApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>
#include <QWaitCondition>
#include <QRegularExpression>
#include <QStringList>

namespace {

QStringList findUrls(const QString& str)
{
    static const QRegularExpression rx(R"(https://[A-Za-z0-9-._~:/?#\[\]@!$&()'*+,;=]+)");

    QStringList lst{};
    QRegularExpressionMatchIterator it = rx.globalMatch(str);
    while (it.hasNext()) {
        auto match = it.next();
        if (match.hasMatch()) {
            lst.push_back(match.captured());
        }
    }
    return lst;
}

}//namespace

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
        QObject::connect(&self, &Worker::finished, reply, &QNetworkReply::deleteLater);

        while (!reply->isFinished()) {
            qApp->processEvents();
        }

        if (reply->error()) {
            request.updater(Request::Status::Error);
            //qDebug() << reply->errorString();
            return;
        }

        const QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if (!redirectUrl.isEmpty()) {
            emit self.urlFound(redirectUrl);
            request.updater(Request::Status::Finished);
            return;
        }

        request.updater(Request::Status::Processing);

        const QString content = QString::fromUtf8(reply->readAll());

        for (const QString& url : findUrls(content)) {
            emit self.urlFound(QUrl(url));
        }

        request.updater(Request::Status::Finished);
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
