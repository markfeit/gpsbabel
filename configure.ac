#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])

#PACKAGE_RELEASE=
AC_INIT([GPSBabel], [1.5.4])
DOCVERSION=1.5.4

# YYYYMMDD, please, if beta, i.e. "-beta20060413"
#DOCVERSION=development


# Increase GBBUILD for a new release (why? Where is this ever used?)
# A: it's used by win32/gpsbabel.rc.in which is used by the setup program,
GBBUILD=28
# PACKAGE_RELEASE="-beta20140216"
AC_DEFINE_UNQUOTED(PACKAGE_RELEASE, "$PACKAGE_RELEASE", [Define to the release name of this package.])
AC_SUBST(PACKAGE_RELEASE)



AC_SUBST(DOCVERSION)

GBMAJOR=`echo $PACKAGE_VERSION | (IFS="."; read major minor micro; echo $major)`
GBMINOR=`echo $PACKAGE_VERSION | (IFS="."; read major minor micro; echo $minor)`
GBMICRO=`echo $PACKAGE_VERSION | (IFS="."; read major minor micro; echo $micro)`
AC_SUBST(GBMAJOR)
AC_SUBST(GBMINOR)
AC_SUBST(GBMICRO)
AC_SUBST(GBBUILD)

AC_CONFIG_SRCDIR([nmea.cc])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AX_CXX_COMPILE_STDCXX([11])
AC_LANG(C++)
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_EXEEXT
AC_SUBST(EXEEXT)
AC_C_BIGENDIAN

# Checks for libraries.
AC_CHECK_LIB([m], [cos])

AS_IF([test "$GCC" = "yes"], [
	CFLAGS="$CFLAGS -Wall"
	CXXFLAGS="$CXXFLAGS -Wall"
])

AC_MSG_CHECKING([whether to support shapefiles])
AC_ARG_ENABLE([shapefile],
	[AS_HELP_STRING(
		[--disable-shapefile],
		[])],
	[],
	[AS_VAR_SET([enable_shapefile], [yes])])
AS_IF([test "$enable_shapefile" = "yes"], [
	AC_DEFINE(SHAPELIB_ENABLED, 1, [1 to enable shapefile support])
])
AC_MSG_RESULT([$enable_shapefile])

AC_MSG_CHECKING([whether to support csv formats])
AC_ARG_ENABLE([csv],
	[AS_HELP_STRING(
		[--disable-csv],
		[])],
	[],
	[AS_VAR_SET([enable_csv], [yes])])
AS_IF([test "$enable_csv" = "yes"], [
	AC_DEFINE(CSVFMTS_ENABLED, 1, [1 to enable the CSV formats support])
])
AC_MSG_RESULT([$enable_csv])

AC_MSG_CHECKING([whether to support maximum number of formats])
AC_ARG_ENABLE([most],
	[AS_HELP_STRING(
		[--disable-most],
		[])],
	[],
	[AS_VAR_SET([enable_most], [yes])])
AS_IF([test "$enable_most" = "yes"], [
	AC_DEFINE(MAXIMAL_ENABLED, 1, [1 to enable as many formats as possible])
	FMTS='$(ALL_FMTS)'
], [
	FMTS='$(MINIMAL_FMTS)'
])
AC_SUBST(FMTS)
AC_MSG_RESULT([$enable_most])

AC_MSG_CHECKING([whether to support filters])
AC_ARG_ENABLE([filters],
	[AS_HELP_STRING(
		[--disable-filters],
		[])],
	[],
	[AS_VAR_SET([enable_filters], [yes])])
AS_IF([test "$enable_filters" = "yes"], [
	AC_DEFINE(FILTERS_ENABLED, 1, [1 to enable all the filters.])
])
AC_MSG_RESULT([$enable_filters])

AC_MSG_CHECKING([whether to support zlib])
AC_ARG_WITH([zlib],
	[AS_HELP_STRING(
		[--with-zlib=(included)|system|no],
		[])])
AS_CASE([$with_zlib], ["system"], [
	AC_CHECK_LIB([z], [gzopen])
], ["no"], [
	AC_DEFINE(ZLIB_INHIBITED, 1, [1 to inhibit our use of zlib.])
	AC_MSG_RESULT([no])
], [
	ZLIB="\$(ZLIB)"
	AC_MSG_RESULT([using included version])
])

AS_CASE(["$target"], [*-*-mingw32*], [
	FILEINFO=fileinfo.o
	AS_IF([test "$CC" = "gcc"], [
		RC=windres
	], [
		RC=`echo "$CC" | sed -e 's/gcc$/windres/'`
	])
], [
	RC=false
])
AC_SUBST(FILEINFO)
AC_SUBST(RC)
# On RHEL/OEL/SL/CENTOS/FEDORA qmake is from Qt3, and qmake-qt4 is from Qt4.
# If qmake-qt4 isn't found look for the standard name qmake and assume it is
# from a useful version Qt.
# On Ubuntu qmake-qt4 exists, but qmake-qt5 does not.
# If we select qmake-qt4 this will defeat qtchooser.
# we no longer support qt4.
AC_CHECK_TOOLS(QMAKE, [qmake-qt5 qmake], "no")
AC_CHECK_TOOLS(LUPDATE, [lupdate-qt5 lupdate])
AC_CHECK_TOOLS(LRELEASE, [lrelease-qt5 lrelease])

AS_IF([test "$QMAKE" = "no"], [
	AC_MSG_ERROR([Qt5.2 or higher is required, but was not found]);
])

# guess the name of the Qt Core library.
QT_LIBVER=$($QMAKE -query -query QT_VERSION | sed -e 's/\..*//')
AS_IF([test "$QT_LIBVER" -ge 5], [
	QT_LIBS="-L$($QMAKE -query QT_INSTALL_LIBS) -lQt${QT_LIBVER}Core"
], [
	QT_LIBS="-L$($QMAKE -query QT_INSTALL_LIBS) -lQtCore"
])
QT_INC=$($QMAKE -query QT_INSTALL_HEADERS)
QT_INC_OPT="-I"
QT_SYSINC_OPT="-isystem"

AS_CASE(["$target"], [*-*-cygwin* | *-*-mingw32*], [
	GBSER=gbser_win.o

	AS_IF([test "$with_libusb" = "no"], [
		AC_MSG_RESULT(USB skipped)
		OSJEEPS=jeeps/gpsusbstub.o
	], [
		OSJEEPS=jeeps/gpsusbwin.o
		USB_LIBS="-lsetupapi -lole32 -lws2_32 -luuid"
		AC_MSG_CHECKING(for Windows DDK)
		old_LIBS="$LIBS"
		LIBS="$LIBS -lhid"
		AC_TRY_LINK([#include <windows.h>]
			[#include <hidsdi.h>],
			[HANDLE              HidDeviceObject;
			HIDD_ATTRIBUTES    Attributes;
			PHIDP_PREPARSED_DATA HidPPD;
			HidD_GetAttributes(HidDeviceObject, &Attributes);
			HidD_GetPreparsedData(HidDeviceObject, &HidPPD);],
			[AC_MSG_RESULT(yes)]
			[AC_DEFINE(HAVE_WDK, 1, [Defined if you have Windows DDK])]
			[USB_LIBS=${USB_LIBS}" -lhid"],
			[AC_MSG_RESULT(no)]
		)
		LIBS="$old_LIBS"
	])
], [*-*-darwin*], [
	GBSER=gbser_posix.o
	OSJEEPS="jeeps/gpslibusb.o \
	mac/libusb/darwin.o \
	mac/libusb/descriptors.o \
	mac/libusb/error.o \
	mac/libusb/usb.o "
	USB_LIBS="-framework  IOKit -framework CoreFoundation"
	AC_DEFINE(HAVE_LIBUSB, 1)

	# On Mac, use frameworks for includes and library files.
	# Macports uses a non-standard build of Qt with an added
	# QT_INSTALL_FRAMEWORKS flag, so look for thier custom
	# version first.  That will fail on 'normal' Qt so fall
	# back to QT_INSTALL_LIBS.
	QT_FW_OR_LIBS=$($QMAKE -query QT_INSTALL_FRAMEWORKS)
	AS_IF([test "$QT_FW_OR_LIBS" = "**Unknown**"], [
		QT_FW_OR_LIBS=$($QMAKE -query QT_INSTALL_LIBS)
	])
	QT_LIBS="-Wl,-rpath, $QT_FW_OR_LIBS -F$QT_FW_OR_LIBS -framework QtCore"
	QT_INC="$QT_FW_OR_LIBS"
	QT_INC_OPT="-F"
	QT_SYSINC_OPT="-iframework"
], [*-*-freebsd*], [
	GBSER=gbser_posix.o
	AC_MSG_CHECKING(for libusb)
	AS_IF([test "$with_libusb" = "no"], [
		AC_MSG_RESULT(check not done)
		OSJEEPS=jeeps/gpsusbstub.o
	], [
		OLDFLAGS=$LDFLAGS
		OCFLAGS=$CFLAGS
		LDFLAGS="$LDFLAGS -lusb"
		CFLAGS="$OCFLAGS"

		AC_CHECK_LIB([usb], [usb_interrupt_read],
			AC_DEFINE(HAVE_LIBUSB, 1, [Defined if you have libusb])
			[USB_CFLAGS=""]
			[USB_LIBS="-lusb"]
			#,[AC_MSG_ERROR([libusb is needed])]
		)
		OSJEEPS=jeeps/gpslibusb.o
		CFLAGS="$OCFLAGS"
	])
], [
	GBSER=gbser_posix.o
	AC_MSG_CHECKING(for libusb)
	AS_IF([test "$with_libusb" = "no"], [
		AC_MSG_RESULT(check not done)
		OSJEEPS=jeeps/gpsusbstub.o
	], [
		AC_CHECK_PROG(LIBUSBCONFIG, libusb-config, true, false)
		AS_IF([test "$LIBUSBCONFIG" = "true"], [
			OLDFLAGS=$LDFLAGS
			OCFLAGS=$CFLAGS
			LDFLAGS="$LDFLAGS `libusb-config --libs`"
			CFLAGS="$OCFLAGS `libusb-config --cflags`"

			AC_CHECK_LIB([usb], [usb_interrupt_read],
				AC_DEFINE(HAVE_LIBUSB, 1, [Defined if you have libusb])
				[USB_CFLAGS="`libusb-config --cflags`"]
				[USB_LIBS="`libusb-config --libs`"]
				#,[AC_MSG_ERROR([libusb >= 0.1.8 is needed])]
			)
			OSJEEPS=jeeps/gpslibusb.o
			CFLAGS="$OCFLAGS"
			#LIBS="$LIBS `libusb-config --libs`"
		], [
			OSJEEPS=jeeps/gpsusbstub.o
		])
	])
])

AC_SUBST(USB_LIBS)
AC_SUBST(USB_CFLAGS)
AC_SUBST(OSJEEPS)
AC_SUBST(GBSER)
AC_SUBST(ZLIB)

dnl Linux HID support
AS_CASE(["$target"], [*-linux-*], [
	AC_MSG_CHECKING(for Linux HID interface)
	AC_COMPILE_IFELSE([AC_LANG_SOURCE([
		#include <sys/ioctl.h>
		#include <linux/types.h>
		#include <linux/hiddev.h>
		#include <linux/hidraw.h>
		void f(void) {
			struct hidraw_devinfo info;
			struct hiddev_field_info finfo;
			struct hiddev_usage_ref_multi urefm;
			struct hiddev_report_info rinfo;
			ioctl(0, HIDIOCGRAWINFO, &info);
			ioctl(0, HIDIOCGFIELDINFO, &finfo);
			ioctl(0, HIDIOCSUSAGES, &urefm);
			ioctl(0, HIDIOCSREPORT, &rinfo);
		}])],
		AC_MSG_RESULT(yes)
		AC_DEFINE(HAVE_LINUX_HID, 1, [Defined if you have Linux HID]),
		AC_MSG_RESULT(no))
])

AS_CASE(["$target"], [*-*-darwin*], [
], [
	AC_MSG_CHECKING(for reduce relocations)
	OCPPFLAGS="$CPPFLAGS"
	CPPFLAGS="$CPPFLAGS -I$QT_INC"
	AC_COMPILE_IFELSE([AC_LANG_SOURCE([
		#include <QtCore/QString>
		void f(void) {
		}])],
		[AC_MSG_RESULT(no)],
		[OCXXFLAGS="$CXXFLAGS"
		CXXFLAGS="$CXXFLAGS -fPIC"
		AC_COMPILE_IFELSE([AC_LANG_SOURCE([
			#include <QtCore/QString>
			void f(void) {
			}])],
			[AC_MSG_RESULT(yes)
			CFLAGS="$CFLAGS -fPIC"],
			[AC_MSG_RESULT(unknown)
			CXXFLAGS="$OCXXFLAGS"]
			)]
		)
	CPPFLAGS="$OCPPFLAGS"
])

AC_MSG_CHECKING([for random stuff to make you feel better])
AC_MSG_RESULT([failed])

AC_MSG_CHECKING([for efence])
AC_ARG_ENABLE([efence],
	[AS_HELP_STRING(
		[--enable-efence],
		[])],
	[AS_IF([test "$enable_efence" != "no"], [
		EFENCE_LIB=-lefence
		GPSBABEL_DEBUG=gpsbabel-debug
		INSTALL_DEBUG=install-debug
	])])
AC_SUBST(EFENCE_LIB)
AC_SUBST(GPSBABEL_DEBUG)
AC_SUBST(INSTALL_DEBUG)
AC_MSG_RESULT([$EFENCE_LIB])

AC_MSG_CHECKING([for docdir])
AC_ARG_WITH([doc],
	[AS_HELP_STRING(
		[--with-doc=DIR],
		[Path where the documentation will be stored.])],
	[DOCDIR="$withval"],
	[DOCDIR="../babelweb/"])
AC_SUBST(DOCDIR)
AC_MSG_RESULT([$DOCDIR])

# Checks for header files.
# AC_HEADER_STDC

# AC_CHECK_HEADERS([fcntl.h inttypes.h libintl.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h strings.h sys/ioctl.h sys/time.h termios.h unistd.h])
# set up HAVE_STDARG_H for zlib.
AC_CHECK_HEADERS([stdarg.h])

# Checks for typedefs, structures, and compiler characteristics.
# AC_C_CONST
# AC_C_INLINE
# AC_TYPE_OFF_T
# AC_TYPE_SIZE_T
# AC_HEADER_TIME
# AC_STRUCT_TM
# AC_STRUCT_TIMEZONE

# Checks for library functions.
# AC_FUNC_MALLOC
# AC_FUNC_MEMCMP
# AC_FUNC_MKTIME
# AC_FUNC_REALLOC
# AC_FUNC_SELECT_ARGTYPES
# AC_FUNC_STRFTIME
# AC_FUNC_STRTOD
# AC_FUNC_VPRINTF
# AC_CHECK_FUNCS([atexit floor localtime_r memmove memset pow select sqrt strchr strcspn strdup strerror strncasecmp strrchr strspn strstr strtol strtoul])
AC_CHECK_FUNCS([nanosleep sleep uname glob])

#
# There's probably some more "autoconfish" way to do this.
#
AC_SUBST(QT_INC)
AC_SUBST(QT_INC_OPT)
AC_SUBST(QT_SYSINC_OPT)
AC_SUBST(QT_LIBS)

AC_CONFIG_FILES([Makefile gbversion.h gui/setup.iss xmldoc/makedoc tools/mkcapabilities win32/gpsbabel.rc jeeps/Makefile shapelib/Makefile zlib/empty])
AC_CONFIG_FILES([gui/makelinuxdist.sh], [chmod +x gui/makelinuxdist.sh])
AC_OUTPUT
