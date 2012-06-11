
MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I../lib \
-I$(top_srcdir)/src/lib \
@my_includes@

bin_PROGRAMS = epeg

epeg_SOURCES = \
epeg_main.c \
epeg_main.h

epeg_LDADD = \
$(top_builddir)/src/lib/libepeg.la

epeg_DEPENDENCIES = $(top_builddir)/src/lib/libepeg.la
