#-------------------------------------------------
# Summary: win32-only wrapper for PDCurses
#
# Simplified and thin Qt project build wrapper
# around PDCurses library for ease of usage with
# other dependent projects on Win32.
# Intent is to reduce amount of efforts to build the
# libary with custom makefiles, and integrate library
# build on Win32 platform with dependent project for
# convenience of usage.
#
#-------------------------------------------------

TARGET = PDCurses
TEMPLATE = lib

# TODO: do not create DLL in target directory
#CONFIG -= dll

DEFINES += PDCURSES_LIBRARY

# Prevent unicode is necessary for now
DEFINES -= UNICODE
DEFINES -= _UNICODE

# ==============================
# Options from makefile
# Debug mode
#DEFINES += "PDCDEBUG=1"

# Wide character set
#DEFINES += "PDC_WIDE=1"

# UTF8 support
#DEFINES += "PDC_FORCE_UTF8=1"
# ==============================

#QMAKE_CFLAGS += -O2 -Wall

# Suppress unwanted warnings
QMAKE_CFLAGS += -Wno-unused-parameter
QMAKE_CFLAGS += -Wno-unused-variable
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-variable


# Redefine custom dir for library binaries
DESTDIR=$$_PRO_FILE_PWD_"/win32/"

# Remove any previous binary leftovers before build
win32: {
    QMAKE_CLEAN += $$DESTDIR"/*.a"
    QMAKE_CLEAN += $$DESTDIR"/*.o"
    QMAKE_CLEAN += $$DESTDIR"/*.exe"
    QMAKE_CLEAN += $$DESTDIR"/*.dll"
    # required flag for "del" - do not prompt for confirmation
    QMAKE_CLEAN += /Q
}

SOURCES += \
    pdcurses/addch.c \
    pdcurses/addchstr.c \
    pdcurses/addstr.c \
    pdcurses/attr.c \
    pdcurses/beep.c \
    pdcurses/bkgd.c \
    pdcurses/border.c \
    pdcurses/clear.c \
    pdcurses/color.c \
    pdcurses/debug.c \
    pdcurses/delch.c \
    pdcurses/deleteln.c \
    pdcurses/deprec.c \
    pdcurses/getch.c \
    pdcurses/getstr.c \
    pdcurses/getyx.c \
    pdcurses/inch.c \
    pdcurses/inchstr.c \
    pdcurses/initscr.c \
    pdcurses/inopts.c \
    pdcurses/insch.c \
    pdcurses/insstr.c \
    pdcurses/instr.c \
    pdcurses/kernel.c \
    pdcurses/keyname.c \
    pdcurses/mouse.c \
    pdcurses/move.c \
    pdcurses/outopts.c \
    pdcurses/overlay.c \
    pdcurses/pad.c \
    pdcurses/panel.c \
    pdcurses/printw.c \
    pdcurses/refresh.c \
    pdcurses/scanw.c \
    pdcurses/scr_dump.c \
    pdcurses/scroll.c \
    pdcurses/slk.c \
    pdcurses/termattr.c \
    pdcurses/terminfo.c \
    pdcurses/touch.c \
    pdcurses/util.c \
    pdcurses/window.c \
    win32/pdcclip.c \
    win32/pdcdisp.c \
    win32/pdcgetsc.c \
    win32/pdckbd.c \
    win32/pdcscrn.c \
    win32/pdcsetsc.c \
    win32/pdcutil.c

HEADERS += \
    curses.h \
    curspriv.h \
    win32/pdcwin.h \
    curses.h \
    curspriv.h \
    term.h

