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

# protobuf
PKG_CHECK_MODULES(
    [protobuf],
    [protobuf >= 2.5.0],
    [protobuf_summary="$protobuf_CFLAGS $protobuf_LIBS"],
    [AC_MSG_ERROR([Required protobuf development files not found!])]
)

# libqb
PKG_CHECK_MODULES(
    [qb],
    [libqb >= 1.0.5],
    [libqb_summary="$qb_CFLAGS $qb_LIBS"],
    [AC_MSG_FAILURE([libqb development files not found])]
)

# usbguard
AC_CHECK_PROGS(USBGUARD, [usbguard])
if test -z "$USBGUARD"; then
    AC_MSG_ERROR(["Required protobuf compiler not found!"])
fi

# protoc
AC_CHECK_PROGS(PROTOC, [protoc])
if test -z "$PROTOC"; then
  AC_MSG_ERROR(["Required protobuf compiler not found!"])
fi


AC_CONFIG_FILES(Makefile)

AC_OUTPUT
