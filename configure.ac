dnl Process this file with autoconf to produce a configure script.

# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT(epeg, 0.9.1.042, enlightenment-devel@lists.sourceforge.net)
AC_PREREQ(2.52)
AC_CONFIG_SRCDIR(configure.ac)

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(config.h)

AC_ISC_POSIX
AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

VMAJ=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $1);}'`
VMIN=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $2);}'`
VMIC=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $3);}'`
SNAP=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $4);}'`
version_info=`expr $VMAJ + $VMIN`":$VMIC:$VMIN"
AC_SUBST(version_info)

dnl AC_CHECK_FUNCS(fmemopen)
dnl AC_CHECK_FUNCS(open_memstream)

AC_CHECK_LIB([m], [log], [], [echo "invalid libm. log not found"])
AC_CHECK_LIB([jpeg], [jpeg_simple_progression], [], [echo "libjpeg library not found. Please install it before proceeding"; exit -1])
AC_CHECK_LIB([exif], [exif_data_new_from_file], [], [echo "libexif library not found. Please install it before proceeding"; exit -1], [-lm])

AC_OUTPUT([
Makefile
epeg.pc
epeg-uninstalled.pc
epeg.spec
src/Makefile
src/lib/Makefile
src/bin/Makefile
debian/changelog
])


#####################################################################
## Info

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo "Configuration Options Summary:"
echo
echo "  Compilation..........: make"
echo
echo "  Installation.........: make install"
echo
echo "    prefix.............: $prefix"
echo
