AC_PREREQ([2.61])

AC_INIT([usbgard-notifier], [0.0.1], [alakatos@redhat.com])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

# if it's possible enable silent compilation
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AM_INIT_AUTOMAKE([foreign subdir-objects tar-pax])

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

AC_CHECK_HEADERS([getopt.h unistd.h], [], [AC_MSG_ERROR([Required header file(s) not found])], [])

# As default each argument is stored temporiraly, however with this options it can be modified
AC_ARG_ENABLE([permanent-notifications],
    [AC_HELP_STRING([--enable-permanent-notifications], [Notifications will be stored permanently(default=no)])],
    [case "${enableval}" in
        yes) notification_path=$prefix ;;
        no) notification_path="/tmp/`id -un`" ;;
        *) AC_MSG_ERROR([Invalid value ${enableval} for --enable-permanent-notifications]) ;;
    esac],
    [notification_path="/tmp/`id -un`"]
)

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

# librsvg2
PKG_CHECK_MODULES(
    [rsvg],
    [librsvg-2.0 >= 2.0.0],
    [librsvg-2.0_summary="$rsvg_CFLAGS $rsvg_LIBS"],
    [AC_MSG_FAILURE([librsvg2 development files not found])]
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

# Catch C++ library
AC_ARG_WITH([bundled-catch], AS_HELP_STRING([--with-bundled-catch],
            [Build using the bundled Catch library]),
            [with_bundled_catch=$withval], [with_bundled_catch=no])
if test "x$with_bundled_catch" = xyes; then
    catch_CFLAGS="-I\$(top_srcdir)/src/ThirdParty/Catch2/single_include/catch2"
    catch_LIBS=""
    AC_MSG_NOTICE([Using bundled Catch library])
    catch_summary="bundled; $catch_CFLAGS $catch_LIBS"
else
    SAVE_CPPFLAGS=$CPPFLAGS
    CPPFLAGS="-std=c++11 $CPPFLAGS -I/usr/include/catch"
    AC_LANG_PUSH([C++])
    AC_CHECK_HEADER([catch.hpp], [], [AC_MSG_FAILURE(catch.hpp not found or not
        usable. Re-run with --with-bundled-catch to use the bundled library.)])
    AC_LANG_POP
    catch_CFLAGS="-I/usr/include/catch"
    catch_LIBS=""
    CPPFLAGS=$SAVE_CPPFLAGS
    catch_summary="system-wide;
    $catch_CFLAGS $catch_LIBS"
fi
AC_SUBST([catch_CFLAGS])
AC_SUBST([catch_LIBS])

CXXFLAGS=
CXXFLAGS+=" -std=c++11"
CXXFLAGS+=" -pedantic"
CXXFLAGS+=" -Wno-unknown-pragmas"
CXXFLAGS+=" -Wall"
CXXFLAGS+=" -Wextra"

AC_SUBST(CPPFLAGS, $CXXFLAGS)
AC_SUBST(config_PATH, $prefix/.config)
AC_SUBST(notification_PATH, $notification_path)

AC_CONFIG_FILES([
    Makefile
    src/Tests/Makefile
])

AC_OUTPUT

echo ""
echo "============== MACROS ================="
echo "          systemd: $systemd_unit_dir"
echo ""
echo "========== LINKER OPTIONS ============="
echo "         protobuf: $protobuf_summary"
echo "            Catch: $catch_summary"
echo "            libqb: $libqb_summary"
echo "        libnotify: $libnotify_summary"
echo "      libusbguard: $libusbguard_summary"
echo ""
echo "============= ARGUMENTS ==============="
echo "           prefix: $prefix"
echo "      config_file: $prefix/.conf"
echo "notification_PATH: $notification_PATH"
echo ""
