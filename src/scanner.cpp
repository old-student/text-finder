#include "scanner.h"
#include <QtDebug>

namespace scan {

Scanner::Scanner()
    : QObject(nullptr)
    , status(Status::NotRunning)
{}

Scanner::~Scanner() = default;

void Scanner::start()
{
    qDebug() << "start";
}

void Scanner::stop()
{
    qDebug() << "stop";
}

void Scanner::pause()
{
    qDebug() << "pause";
}

}// namespace scan
