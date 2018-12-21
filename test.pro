QT += quick network
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

INCLUDEPATH += \
        $$PWD/src \
        $$PWD/src/common \
        $$PWD/src/threadpool \
        $$PWD/src/qmlproxy

HEADERS += \
        src/common/types.h \
        src/threadpool/worker.h \
        src/threadpool/thread.h \
        src/threadpool/threadpool.h \
        src/qmlproxy/report.h \
        src/qmlproxy/scanner.h \

SOURCES += \
        src/threadpool/worker.cpp \
        src/threadpool/thread.cpp \
        src/threadpool/threadpool.cpp \
        src/qmlproxy/report.cpp \
        src/qmlproxy/scanner.cpp \
        src/main.cpp

RESOURCES += qml.qrc
