QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
        src/threadpool/worker.h \
        src/threadpool/thread.h \
        src/threadpool/threadpool.h \
        src/scanner.h \

SOURCES += \
        src/threadpool/worker.cpp \
        src/threadpool/thread.cpp \
        src/threadpool/threadpool.cpp \
        src/scanner.cpp \
        src/main.cpp

RESOURCES += qml.qrc
