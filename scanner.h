#ifndef __EHNGRFHIDTJBOYPXKCTNPVWDKQSUGOKKGAVSOAYS__
#define __EHNGRFHIDTJBOYPXKCTNPVWDKQSUGOKKGAVSOAYS__

#include <QObject>
#include <QString>

namespace scan {

class Scanner : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString startUrl MEMBER startUrl NOTIFY startUrlChanged)
    Q_PROPERTY(int threadsNumber MEMBER threadsNumber NOTIFY threadsNumberChanged)
    Q_PROPERTY(QString searchText MEMBER searchText NOTIFY searchTextChanged)
    Q_PROPERTY(int urlLimit MEMBER urlLimit NOTIFY urlLimitChanged)
    Q_PROPERTY(Status status READ getStatus NOTIFY statusChanged)

public:
    enum class Status
    {
        NotRunning,
        Running,
        Paused
    };
    Q_ENUM(Status)

    Scanner();
    ~Scanner();

public:
    Status getStatus() const
    {
        return status;
    }

private:
    void setStatus(const Status status)
    {
        this->status = status;
        emit statusChanged();
    }

signals:
    void startUrlChanged();
    void threadsNumberChanged();
    void searchTextChanged();
    void urlLimitChanged();
    void statusChanged();

public slots:
    void start();
    void stop();
    void pause();

private:    
    Status status;
    QString startUrl;
    int threadsNumber;
    QString searchText;
    int urlLimit;
};

}// namespace scan

#endif // __EHNGRFHIDTJBOYPXKCTNPVWDKQSUGOKKGAVSOAYS__