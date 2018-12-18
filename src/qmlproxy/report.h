#ifndef __VPEAESJQZCPVGKTKWVYGJATXJBGACCYOSGBGAEFB__
#define __VPEAESJQZCPVGKTKWVYGJATXJBGACCYOSGBGAEFB__

#include <QAbstractListModel>
#include <QVariant>
#include <QUrl>
#include <memory>

namespace scan {

class ReportModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum class Status
    {
        Pending,
        Downloading,
        Redirected,
        Finished,
        Error
    };

    explicit ReportModel(QObject* parent = nullptr);
    virtual ~ReportModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    int addEntry(const QUrl& url);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __VPEAESJQZCPVGKTKWVYGJATXJBGACCYOSGBGAEFB__
