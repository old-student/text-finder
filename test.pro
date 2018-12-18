QT += quick
CONFIG += c++11

DEFINES += QT_DEPRECATED_WARNINGS

HEADERS += \
        src/threadpool/worker.h \
        scanner.h \
        task.h

SOURCES += \
        src/threadpool/worker.cpp \
        main.cpp \
        scanner.cpp

RESOURCES += qml.qrc
