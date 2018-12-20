#ifndef __KDJSKDJSDJSDKSJDSKDJSKDJSKDSJDKDJSKD__
#define __KDJSKDJSDJSDKSJDSKDJSKDJSKDSJDKDJSKD__

#include <QMetaType>
#include <QUrl>
#include <functional>

namespace scan {

struct Request
{
    enum class Status
    {
        Pending,
        Downloading,
        Error,
        Processing,
        Finished
    };

    using Updater = std::function<void(Status)>;

    Request()
    {}

    Request(QUrl url, Updater updater = [](Status){})
        : url(url), updater(updater)
    {}

    QUrl url;
    Updater updater;
};

}// namespace scan

Q_DECLARE_METATYPE(scan::Request)
Q_DECLARE_METATYPE(scan::Request::Status)

#endif // __KDJSKDJSDJSDKSJDSKDJSKDJSKDSJDKDJSKD__
