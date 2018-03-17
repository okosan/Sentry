QT += core
QT -= gui

CONFIG += c++11

TARGET = sentry
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

# Include PDCurses
INCLUDEPATH += $$_PRO_FILE_PWD_"/thirdparty/PDCurses"
win32:LIBS += -L$$_PRO_FILE_PWD_"/thirdparty/PDCurses/win32" -lpdcurses
unix:LIBS += -L$$_PRO_FILE_PWD_"/thirdparty/PDCurses/x11" -lpdcurses


SOURCES += main.cpp \
    xobjectbug.cpp \
    sound.cpp \
    world.cpp \
    xobjectpress.cpp \
    xobjectghost.cpp \
    xobjectplayerhero.cpp \
    input.cpp \
    xobjectrobik.cpp \
    render.cpp \
    xdiscoverymap.cpp \
    xgamesettings.cpp \
    xfogofviewmap.cpp \
    xdynamicobject.cpp \
    xobjectcloud.cpp \
    utils.cpp \
    xobjectcarrier.cpp \
    xobjectinterceptors.cpp \
    xobjectfox.cpp

HEADERS += \
    sound.h \
    world.h \
    input.h \
    render.h \
    xobjectbug.h \
    xobjectpress.h \
    xobjectghost.h \
    xobjectplayerhero.h \
    xobjectrobik.h \
    xdiscoverymap.h \
    xgamesettings.h \
    xfogofviewmap.h \
    xdynamicobject.h \
    xobjectcloud.h \
    utils.h \
    xobjectcarrier.h \
    xobjectinterceptors.h \
    xobjectfox.h
