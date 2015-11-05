TEMPLATE = app
QT += widgets
INCLUDEPATH += $$PWD/../common
CONFIG += c++11

include(../../lib/rpc.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    ../common/user.cpp \
    ../common/server.cpp

FORMS += \
    mainwindow.ui

HEADERS += \
    mainwindow.h \
    ../common/user.h \
    ../common/server.h \
    ../common/defines.h

