AC_PREREQ([2.61])

AC_INIT([usbgard_notifier], [0.1], [alakatos@redhat.com])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

# if it's possible enable silent compilation
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([foreign subdir-objects])

AC_PROG_MAKE_SET
AC_PROG_CXX
AC_PROG_CC

AC_PROG_INSTALL
AC_PROG_MAKE_SET
AM_PROG_LIBTOOL

# Check for headers, typedefs, data structures
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_CHECK_HEADERS([getopt.h unistd.h] ,[], [AC_MSG_ERROR([Required header file(s) not found])], [])

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

# libnotify
PKG_CHECK_MODULES(
    [notify],
    [libnotify >= 0.7.0],
    [libnotify_summary="$notify_CFLAGS $notify_LIBS"],
    [AC_MSG_FAILURE([libnotify development files not found])]
)

# usbguard
PKG_CHECK_MODULES(
    [usbguard],
    [libusbguard >= 0.7.2],
    [libusbguard_summary="$usbguard_CFLAGS $usbguard_LIBS"],
    [AC_MSG_FAILURE([libusbguard development files not found])]
)

# protoc
AC_CHECK_PROGS(PROTOC, [protoc])
if test -z "$PROTOC"; then
  AC_MSG_ERROR(["Required protoc compiler not found!"])
fi

# asciidoc
AC_CHECK_PROGS(A2X, [a2x])
if test -z "$A2X"; then
  AC_MSG_FAILURE([Cannot detect documentation generator (asciidoc / a2x)])
fi

# systemd
systemd_unit_dir="`$PKG_CONFIG systemd --variable=systemduserunitdir`"
if test "x$systemd_unit_dir" = x; then
    AC_MSG_FAILURE([Cannot detect the systemd system unit dir])
fi
AC_SUBST(SYSTEMD_UNIT_DIR, $systemd_unit_dir)
AC_DEFINE([SYSTEMD_SUPPORT_ENABLED], [1], [Enable systemd support int the project])
AM_CONDITIONAL([SYSTEMD_SUPPORT_ENABLED], [test "x$systemd" = xyes ])

CXXFLAGS=
CXXFLAGS+=" -std=c++11"
CXXFLAGS+=" -pedantic"
CXXFLAGS+=" -Wno-unknown-pragmas"
CXXFLAGS+=" -Wall"
CXXFLAGS+=" -Wextra"

AC_SUBST(CPPFLAGS, $CXXFLAGS)

AC_CONFIG_FILES(Makefile)

AC_OUTPUT

echo ""
echo "========== MACROS =============="
echo "    systemd: $systemd_unit_dir"
echo ""
echo "======== LINKER OPTIONS ========"
echo "   protobuf: $protobuf_summary"
echo "      libqb: $libqb_summary"
echo "  libnotify: $libnotify_summary"
echo "libusbguard: $libusbguard_summary"
echo ""
