#include "report.h"
#include <QString>
#include <QVector>
#include <QtDebug>

namespace scan {

struct ReportModel::Impl
{
    struct ReportEntry
    {
        QUrl url;
        Request::Status status;
        QString description;
    };

    enum Roles
    {
        UrlRole = Qt::UserRole + 1,
        StatusRole,
        DescriptionRole
    };

    static QString toString(const Request::Status status)
    {
        static const QMap<Request::Status, QString> map =
        {
            { Request::Status::Pending    , "Pending"     },
            { Request::Status::Downloading, "Downloading" },
            { Request::Status::Error      , "Error"       },
            { Request::Status::Processing , "Processing"  },
            { Request::Status::Finished   , "Finished"    }
        };
        return map[status];
    }

    // data members
    QVector<ReportEntry> data;
};

ReportModel::ReportModel(QObject* parent)
    : QAbstractListModel(parent)
    , impl(new Impl())
{}

ReportModel::~ReportModel() = default;

int ReportModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return impl->data.size();
}

QVariant ReportModel::data(const QModelIndex &index, int role) const
{
    const auto idx = index.row();
    if (idx < 0 || idx >= impl->data.size()) { return QVariant{}; }

    QVariant value{};
    switch (role) {
        case Impl::UrlRole:
            value = impl->data[idx].url;
            break;
        case Impl::StatusRole:
            value = Impl::toString(impl->data[idx].status);
            break;
        case Impl::DescriptionRole:
            value = impl->data[idx].description;
            break;
    }
    return value;
}

QHash<int, QByteArray> ReportModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Impl::UrlRole]   = "url";
    roles[Impl::StatusRole] = "status";
    roles[Impl::DescriptionRole] = "description";
    return roles;
}

void ReportModel::clear()
{
    beginResetModel();
    impl->data.clear();
    endResetModel();
}

Request::Updater ReportModel::registerRequest(const QUrl& url)
{
    int i = impl->data.size();
    beginInsertRows(QModelIndex(), i, i);
    impl->data.push_back({url, Request::Status::Pending, QString("")});
    endInsertRows();
    return [this,i](Request::Status status, QString description) {
        QMetaObject::invokeMethod(this,
                                  "updateData",
                                  Q_ARG(int, i),
                                  Q_ARG(Request::Status, status),
                                  Q_ARG(QString, description)
                                  );
    };
}

void ReportModel::updateData(int i, Request::Status status, QString description)
{
    impl->data[i].status = status;
    impl->data[i].description = description;
    dataChanged(index(i), index(i));
}

}//namespace scan
