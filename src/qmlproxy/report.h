#ifndef __VPEAESJQZCPVGKTKWVYGJATXJBGACCYOSGBGAEFB__
#define __VPEAESJQZCPVGKTKWVYGJATXJBGACCYOSGBGAEFB__

#include "types.h"
#include <QAbstractListModel>
#include <memory>

namespace scan {

class ReportModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit ReportModel(QObject* parent = nullptr);
    ~ReportModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void clear();
    Request::Updater registerRequest(const QUrl& url);
    void updateData(int i, Request::Status status, QString description);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;
};

}//namespace scan

#endif // __VPEAESJQZCPVGKTKWVYGJATXJBGACCYOSGBGAEFB__
