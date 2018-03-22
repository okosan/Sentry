# GNU MAKE Makefile for PDCurses library - WIN32 MinGW GCC
#
# Usage: make -f [path\]mingwin32.mak [DEBUG=Y] [DLL=Y] [WIDE=Y] [UTF8=Y] [tgt]
#
# where tgt can be any of:
# [all|pdcurses.a|clean]

O = o

ifndef PDCURSES_SRCDIR
	PDCURSES_SRCDIR = ..
endif

include $(PDCURSES_SRCDIR)/version.mif
include $(PDCURSES_SRCDIR)/libobjs.mif

osdir		= $(PDCURSES_SRCDIR)/win32

PDCURSES_WIN_H	= $(osdir)/pdcwin.h

CC		= gcc

ifeq ($(DEBUG),Y)
	CFLAGS  = -g -Wall -DPDCDEBUG
else
	CFLAGS  = -O2 -Wall
endif

CFLAGS += -I$(PDCURSES_SRCDIR)

BASEDEF		= $(PDCURSES_SRCDIR)\exp-base.def
WIDEDEF		= $(PDCURSES_SRCDIR)\exp-wide.def

DEFDEPS		= $(BASEDEF)

ifeq ($(WIDE),Y)
	CFLAGS += -DPDC_WIDE
	DEFDEPS += $(WIDEDEF)
endif

ifeq ($(UTF8),Y)
	CFLAGS += -DPDC_FORCE_UTF8
endif

DEFFILE		= pdcurses.def

LINK		= gcc

ifeq ($(DLL),Y)
	CFLAGS += -DPDC_DLL_BUILD
	LIBEXE = gcc $(DEFFILE)
	LIBFLAGS = -Wl,--out-implib,pdcurses.a -shared -o
	LIBCURSES = pdcurses.dll
	LIBDEPS = $(LIBOBJS) $(PDCOBJS) $(DEFFILE)
	CLEAN = $(LIBCURSES) *.a $(DEFFILE)
else
	LIBEXE = ar
	LIBFLAGS = rcv
	LIBCURSES = pdcurses.a
	LIBDEPS = $(LIBOBJS) $(PDCOBJS)
	CLEAN = *.a
endif

.PHONY: all libs clean objclean

all:	clean libs objclean
	-del *.o
	-del *.exe

libs:	clean $(LIBCURSES) objclean

objclean:
	-del *.o
	-del *.exe

clean:	objclean
	-del $(CLEAN)


$(LIBCURSES) : $(LIBDEPS)
	$(LIBEXE) $(LIBFLAGS) $@ $?

$(LIBOBJS) $(PDCOBJS) : $(PDCURSES_HEADERS)
$(PDCOBJS) : $(PDCURSES_WIN_H)

$(LIBOBJS) : %.o: $(srcdir)/%.c
	$(CC) -c $(CFLAGS) $<

$(PDCOBJS) : %.o: $(osdir)/%.c
	$(CC) -c $(CFLAGS) $<

