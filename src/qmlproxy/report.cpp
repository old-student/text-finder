#include "report.h"
#include <QString>
#include <QVector>

namespace scan {

struct ReportModel::Impl
{
    enum Roles
    {
        UrlRole = Qt::UserRole + 1,
        StatusRole
    };

    struct ReportEntry
    {
        QUrl url;
        Status status;
    };

    static QString toString(const Status status)
    {
        static const QMap<Status, QString> map =
        {
            { Status::Pending    , "Pending"     },
            { Status::Downloading, "Downloading" },
            { Status::Redirected , "Redirected"  },
            { Status::Finished   , "Finished"    },
            { Status::Error      , "Error"       }
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
    if (idx < 0 || idx >= impl->data.size()) { return QVariant{};}

    QVariant value{};
    switch (role) {
        case Impl::UrlRole:
            value = impl->data[idx].url;
            break;
        case Impl::StatusRole:
            value = Impl::toString(impl->data[idx].status);
            break;
    }
    return value;
}

QHash<int, QByteArray> ReportModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Impl::UrlRole]   = "url";
    roles[Impl::StatusRole] = "status";
    return roles;
}

int ReportModel::addEntry(const QUrl& url)
{
    int i = impl->data.size();
    beginInsertRows(QModelIndex(), i, i);
    impl->data.push_back({url, Status::Pending});
    endInsertRows();
    return i;
}

}//namespace scan
