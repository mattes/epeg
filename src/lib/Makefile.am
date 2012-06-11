
MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS	     = @my_includes@

lib_LTLIBRARIES      = libepeg.la
include_HEADERS      = Epeg.h
libepeg_la_SOURCES  = \
epeg_main.c \
epeg_private.h

libepeg_la_LIBADD       = @my_libs@
libepeg_la_DEPENDENCIES = $(top_builddir)/config.h
libepeg_la_LDFLAGS      = -version-info @version_info@
