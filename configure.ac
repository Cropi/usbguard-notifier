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

PKG_CHECK_MODULES([protobuf], [protobuf >= 2.5.0],
[protobuf_summary="system-wide; $protobuf_CFLAGS $protobuf_LIBS"],
[AC_MSG_ERROR([Required protobuf development files not found!])]
)

AC_CHECK_PROGS(PROTOC, [protoc])
if test -z "$PROTOC"; then
  AC_MSG_ERROR(["Required protobuf compiler not found!"])
fi

AC_CONFIG_FILES(Makefile)

AC_OUTPUT
