
MAINTAINERCLEANFILES = Makefile.in

lib_LTLIBRARIES      = libepeg.la
include_HEADERS      = Epeg.h
libepeg_la_SOURCES  = \
epeg_main.c \
epeg_private.h

libepeg_la_DEPENDENCIES = $(top_builddir)/config.h
libepeg_la_LDFLAGS      = -version-info @version_info@
