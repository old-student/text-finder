#include "report.h"
#include <QString>
#include <QVector>

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

    static QString statusToString(const Request::Status status)
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

    Impl(ReportModel& self)
        : self(self)
    {}

    ~Impl() = default;

    // data members
    ReportModel& self;
    QVector<ReportEntry> data;
};

ReportModel::ReportModel(QObject* parent)
    : QAbstractListModel(parent)
    , impl(std::make_shared<Impl>(*this))
{}

ReportModel::~ReportModel() = default;

int ReportModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return impl->data.size();
}

QVariant ReportModel::data(const QModelIndex& index, int role) const
{
    const auto idx = index.row();
    if (idx < 0 || idx >= impl->data.size()) { return QVariant{}; }

    QVariant value{};
    switch (role) {
        case Impl::UrlRole:
            value = impl->data[idx].url;
            break;
        case Impl::StatusRole:
            value = Impl::statusToString(impl->data[idx].status);
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
    roles[Impl::UrlRole]         = "url";
    roles[Impl::StatusRole]      = "status";
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
    const int index = impl->data.size();
    beginInsertRows(QModelIndex(), index, index);
    impl->data.push_back({url, Request::Status::Pending, QString("")});
    endInsertRows();

    // create and return callback for updating during processing
    std::weak_ptr<Impl> wp = impl;
    return [wp, index](Request::Status status, QString description) {
        if (auto sp = wp.lock()) {
            QMetaObject::invokeMethod(&sp->self,
                                      "updateData",
                                      Q_ARG(int, index),
                                      Q_ARG(Request::Status, status),
                                      Q_ARG(QString, description)
                                      );
        }
    };
}

void ReportModel::updateData(int index, Request::Status status, QString description)
{
    impl->data[index].status = status;
    impl->data[index].description = description;
    dataChanged(index(index), index(index));
}

}//namespace scan
