#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([makerscanner], [0.3.2], [abarry@makerscanner.com])
AM_INIT_AUTOMAKE(makerscanner, 0.3.2)
AC_CONFIG_SRCDIR([ScanStatus.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.

##########################################################################
# check for libgsl headers
##########################################################################

# use the C++ compiler for the following checks
AC_LANG([C++])

# store current CXXFLAGS and merge with AM_CXXFLAGS for compilation check
OLD_CXXFLAGS=$CXXFLAGS;
CXXFLAGS="$AM_CXXFLAGS $CXXFLAGS"

# check for libgsl library headers
AC_MSG_CHECKING([for the libgsl library headers])
# try to compile a file that includes a header of the library XXX
AC_COMPILE_IFELSE([AC_LANG_SOURCE([[#include <gsl/gsl_spline.h>]])],
    [AC_MSG_RESULT([found])
        FOUND_GSL=1;],
    [AC_MSG_RESULT([not found])
        FOUND_GSL=0;])

# reset original CXXFLAGS
CXXFLAGS=$OLD_CXXFLAGS

# handle results
if test $FOUND_GSL != 1; then
    AC_MSG_NOTICE([])
    AC_MSG_NOTICE([The libgsl library was not found!])

    AC_MSG_NOTICE([])
    AC_MSG_ERROR([
        libgsl must be installed on your system.

        If you are on Ubuntu you can probably solve this by running

        sudo apt-get install libgsl0-dev
        ])
fi

##########################################################################



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
