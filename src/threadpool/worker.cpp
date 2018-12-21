#include "worker.h"
#include <QGuiApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QMutex>
#include <QWaitCondition>
#include <QRegularExpression>
#include <QStringList>
#include <QTimer>

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

size_t findText(const QString& searchText, const QString& str)
{
    size_t count = 0;
    int index = str.indexOf(searchText);
    while (index >= 0) {
        ++count;
        index = str.indexOf(searchText, index + searchText.size());
    }
    return count;
}

}//namespace

namespace scan {

struct Worker::Impl
{
    Impl(Worker& self)
        : networkManager(new QNetworkAccessManager(&self))
        , timeoutTimer(new QTimer(&self))
        , self(self)
    {
        timeoutTimer->setSingleShot(true);
        timeoutTimer->setInterval(5000);
    }

    void processRequest(const Request& request)
    {
        if (networkManager->networkAccessible() != QNetworkAccessManager::Accessible) {
            request.updater(Request::Status::Error, "network is not accessible");
            return;
        }

        request.updater(Request::Status::Downloading, QString("0 %"));
        QNetworkReply* reply = networkManager->get(QNetworkRequest(request.url));
        QObject::connect(&self, &Worker::finished, reply, &QNetworkReply::deleteLater);
        QObject::connect(reply, &QNetworkReply::downloadProgress,
            [request](qint64 bytesReceived, qint64 totalBytes) {
                request.updater(Request::Status::Downloading,
                                totalBytes > 0
                                ? QString::number(bytesReceived * 100.0f / totalBytes, 'g', 2) + " %"
                                : QString("0 %"));
        });

        timeoutTimer->stop();
        timeoutTimer->start();

        while (!reply->isFinished()) {
            qApp->processEvents();
            if (!timeoutTimer->isActive() && reply->bytesAvailable() == 0) {
                reply->abort();
                request.updater(Request::Status::Error, "request timeout exceeded");
                return;
            }
        }

        if (reply->error()) {
            request.updater(Request::Status::Error, reply->errorString());
            return;
        }

        const QUrl redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
        if (!redirectUrl.isEmpty()) {
            emit self.urlFound(redirectUrl);
            request.updater(Request::Status::Finished, "redirect to " + redirectUrl.toString());
            return;
        }

        request.updater(Request::Status::Processing, "");

        const QString content = QString::fromUtf8(reply->readAll());

        for (const QString& url : findUrls(content)) {
            emit self.urlFound(QUrl(url));
        }

        const size_t n = findText(request.searchText, content);
        request.updater(Request::Status::Finished,
                        n > 0 ? QString("search text found %1 times").arg(n)
                              : QString("search text not found"));
    }

    // data members
    QNetworkAccessManager* networkManager;
    QTimer* timeoutTimer;
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

void Worker::stop()
{
    QMetaObject::invokeMethod(this, "stopImpl");
}

void Worker::stopImpl()
{
    impl->timeoutTimer->stop();
}

void Worker::processRequest(Request request)
{
    impl->processRequest(request);
    emit finished(this);
}

}//namespace scan
