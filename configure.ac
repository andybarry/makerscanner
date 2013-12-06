#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([makerscanner], [0.3.1], [abarry@makerscanner.com])
AM_INIT_AUTOMAKE(makerscanner, 0.3.1)
AC_CONFIG_SRCDIR([ScanStatus.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
## check for libcv
#AC_CHECK_LIB(cv,
#	main,
#	LIBS="$LIBS -lcv", 
#	[AC_MSG_ERROR([
#		*** libcv (OpenCV) library not found!
#		You need OpenCV (http://opencv.willowgarage.com)
#		
#		If you are running Ubuntu you can probably fix this by running
#		
#		sudo apt-get install libcv1 libcv-dev libcvaux1 libcvaux-dev libhighgui1 libhighgui-dev
#		
#		])]
#)

PKG_CHECK_MODULES([OPENCV], [opencv >= 2.0])



# check for wxWidgets

m4_include(wxwin.m4)

AM_OPTIONS_WXCONFIG
reqwx=2.4.0
AM_PATH_WXCONFIG($reqwx, wxWin=1)
if test "$wxWin" != 1; then
	AC_MSG_ERROR([
		wxWidgets must be installed on your system.
 
		Please check that wx-config is in path, the directory
		where wxWidgets libraries are installed (returned by
		'wx-config --libs' or 'wx-config --static --libs' command)
		is in LD_LIBRARY_PATH or equivalent variable and
		wxWidgets version is $reqwx or above.
		
		If you are on Ubuntu you can probably solve this by running
		sudo apt-get install libwxgtk2.8-dev
		])
fi

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY"
CFLAGS="$CFLAGS $WX_CFLAGS_ONLY"
LIBS="$LIBS $WX_LIBS"

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
