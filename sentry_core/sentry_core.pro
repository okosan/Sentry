QT -= core
QT -= gui

CONFIG += c++11

TARGET = sentry
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

############### Dependencies #################
# Include PDCurses library needed for 2D ASCII based drawing
INCLUDEPATH += $$_PRO_FILE_PWD_"/../thirdparty/PDCurses"
# Windows users like when dependencies are
# resolved magically with unnecessary efforts
win32:LIBS += -L$$_PRO_FILE_PWD_"/../thirdparty/PDCurses/win32" -lpdcurses
# While Linux users prefer to build everything themselfs..
unix:LIBS += -L$$_PRO_FILE_PWD_"/../thirdparty/PDCurses/x11" -lpdcurses


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
    xobjectfox.cpp \
    world_pathfinder.cpp \
    pathfinder/cell.cpp \
    pathfinder/map.cpp \
    pathfinder/pathfinder.cpp

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
    xobjectfox.h \
    pathfinder/arr2d.h \
    pathfinder/arr2d_impl.h \
    pathfinder/cell.h \
    pathfinder/map.h \
    pathfinder/pathfinder.h
