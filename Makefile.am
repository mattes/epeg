## Process this file with automake to produce Makefile.in

SUBDIRS = src

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess \
                       config.h.in config.sub configure install-sh \
		       ltconfig ltmain.sh missing mkinstalldirs \
		       stamp-h.in epeg.c depcomp \
		       epeg.spec debian/changelog

EXTRA_DIST = README AUTHORS COPYING COPYING-PLAIN \
	     autogen.sh epeg.spec epeg.spec.in epeg.c.in gendoc \
	     Doxyfile \
             debian/changelog \
             debian/changelog.in \
             debian/control \
             debian/copyright \
             debian/libepeg0-dev.install \
	     debian/epeg0-bin.install \
             debian/libepeg0.install \
             debian/rules

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = epeg.pc
