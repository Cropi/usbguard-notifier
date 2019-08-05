AC_PREREQ([2.61])

AC_INIT([usbgard_notifier], [0.1], [alakatos@redhat.com])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_MAKE_SET
AC_PROG_CXX
AC_PROG_CC

AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL

CXXFLAGS =" -std=c++11"
CXXFLAGS+=" -Wall"
CXXFLAGS+=" -pedantic"
CXXFLAGS+=" -Werror -pthread -lprotobuf -lpthread"
CXXFLAGS+=" -lqb -ldl"


AC_CONFIG_FILES(Makefile)

AC_OUTPUT
