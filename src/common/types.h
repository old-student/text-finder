#ifndef __EXJNBIVRHIUXDCSXGAFBQBDMFPOKHVHATUQENGBA__
#define __EXJNBIVRHIUXDCSXGAFBQBDMFPOKHVHATUQENGBA__

#include <QMetaType>
#include <QUrl>
#include <QString>
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

    using Updater = std::function<void(Status, QString)>;

    Request()
    {}

    Request(const QUrl& url,
            const QString& searchText,
            Updater updater = [](Status, QString){})
        : url(url), searchText(searchText), updater(updater)
    {}

    QUrl url;
    QString searchText;
    Updater updater;
};

}// namespace scan

Q_DECLARE_METATYPE(scan::Request)
Q_DECLARE_METATYPE(scan::Request::Status)

#endif // __EXJNBIVRHIUXDCSXGAFBQBDMFPOKHVHATUQENGBA__
