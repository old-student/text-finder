#ifndef __EHNGRFHIDTJBOYPXKCTNPVWDKQSUGOKKGAVSOAYS__
#define __EHNGRFHIDTJBOYPXKCTNPVWDKQSUGOKKGAVSOAYS__

#include "threadpool.h"
#include "report.h"
#include <QObject>
#include <QString>

namespace scan {

class Scanner : public QObject
{
    Q_OBJECT

    Q_PROPERTY(ReportModel* reportModel READ getReportModel CONSTANT)
    Q_PROPERTY(QString startUrl MEMBER startUrl NOTIFY startUrlChanged)
    Q_PROPERTY(int threadCount MEMBER threadCount NOTIFY threadCountChanged)
    Q_PROPERTY(QString searchText MEMBER searchText NOTIFY searchTextChanged)
    Q_PROPERTY(int requestLimit MEMBER requestLimit NOTIFY requestLimitChanged)
    Q_PROPERTY(Status status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(float progressValue READ getProgressValue NOTIFY progressValueChanged)

public:
    enum class Status
    {
        NotRunning,
        Running,
        Suspended,
        Finished
    };
    Q_ENUM(Status)

    explicit Scanner(QObject* parent = nullptr);
    ~Scanner();

public:
    ReportModel* getReportModel()
    {
        return reportModel;
    }

    Status getStatus() const
    {
        return status;
    }

    float getProgressValue() const;

private:
    void setStatus(const Status status)
    {
        this->status = status;
        emit statusChanged();
    }

signals:
    void startUrlChanged();
    void threadCountChanged();
    void searchTextChanged();
    void requestLimitChanged();
    void statusChanged();
    void progressValueChanged();

public slots:
    void start();
    void suspend();
    void resume();
    void stop();

private:
    ReportModel* reportModel;
    ThreadPool pool;
    Status status;
    QString startUrl;
    int threadCount;
    QString searchText;
    int requestLimit;
};

}// namespace scan

#endif // __EHNGRFHIDTJBOYPXKCTNPVWDKQSUGOKKGAVSOAYS__
