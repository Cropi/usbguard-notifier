AC_PREREQ([2.71])

AC_INIT([usbguard-notifier],[0.1.1],[alakatos@redhat.com])
AC_CONFIG_SRCDIR([src/])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])
m4_pattern_allow([AC_MSG_FAILURE])

# if it's possible enable silent compilation
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

EXTERNAL_CXXFLAGS="$CXXFLAGS"

AM_INIT_AUTOMAKE([foreign subdir-objects tar-pax])

AC_PROG_MAKE_SET
AC_PROG_CXX
AC_PROG_CC

AC_PROG_INSTALL
LT_PREREQ([2.4.6])
LT_INIT

# Check for headers, typedefs, data structures
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

AC_CHECK_HEADERS([stdbool.h getopt.h unistd.h], [], [AC_MSG_ERROR([Required header file(s) not found])], [])

# As default each argument is stored temporarily, however with this option it can be modified
notification_path="/tmp/usbguard-notifier"
AC_ARG_WITH(
    [permanent-notifications],
    AS_HELP_STRING([--with-permanent-notifications], [Select notifications path]),
    [notification_path=$withval],
    [notification_path="/tmp/usbguard-notifier"]
)

# usbguard-devel
# Add the path to where your usbguard-devel includes are
# You might need this option when you want to package usbguard-notifier
# together with usbguard at the same time
AC_ARG_WITH(
    [usbguard-devel],
    AS_HELP_STRING([--with-usbguard-devel], [Select to compile notifier from source usbguard devel files(only top level directory)]),
    [usbguard_CFLAGS="-I$withval/src/Library/public/"
     usbguard_LIBS=""
     usbguard_LA="$withval/libusbguard.la"
     libusbguard_summary="$usbguard_CFLAGS $usbguard_LIBS"
     AC_SUBST([usbguard_CFLAGS])
     AC_SUBST([usbguard_LIBS])
     AC_SUBST([usbguard_LA])
     custom_usbguard_devel_enabled=yes
    ],
    [
        PKG_CHECK_MODULES(
            [usbguard],
            [libusbguard >= 0.7.2],
            [libusbguard_summary="$usbguard_CFLAGS $usbguard_LIBS"],
            [AC_MSG_FAILURE([libusbguard development files not found])]
        )
    ]
)

# Build notifier-cli, default is yes
AC_ARG_ENABLE([notifier-cli],
    [AS_HELP_STRING([--enable-notifier-cli], [enable notifier cli(default=yes)])],
    [case "${enableval}" in
        yes) notifier_cli_enabled=yes;;
        no) notifier_cli_enabled=no;;
        *) AC_MSG_ERROR([bad value ${enableval} for --enable-notifier-cli]) ;;
     esac],
    [notifier_cli_enabled=yes]
)

AC_ARG_ENABLE([systemd-systemunitdir],
    [AS_HELP_STRING([--enable-systemd-systemunitdir],[enable systemd system unit dir(default=no)])],
    [case "${enableval}" in
        yes) systemd_systemunitdir=yes;;
        no) systemd_systemunitdir=no;;
        *) AC_MSG_ERROR([bad value ${enableval} for --enable-systemd-systemunitdir]) ;;
     esac],
    [systemd_systemunitdir=no]
)

# librsvg2
PKG_CHECK_MODULES(
    [rsvg],
    [librsvg-2.0 >= 2.0.0],
    [librsvg_summary="$rsvg_CFLAGS $rsvg_LIBS"],
    [AC_MSG_FAILURE([librsvg2 development files not found])]
)

# libnotify
PKG_CHECK_MODULES(
    [notify],
    [libnotify >= 0.7.0],
    [libnotify_summary="$notify_CFLAGS $notify_LIBS"],
    [AC_MSG_FAILURE([libnotify development files not found])]
)

# asciidoc
AC_CHECK_PROGS(A2X, [a2x])
if test -z "$A2X"; then
  AC_MSG_FAILURE([Cannot detect documentation generator (asciidoc / a2x)])
fi

# systemd
if test "x$systemd_systemunitdir" = xyes; then
    systemd_unit_dir="`$PKG_CONFIG systemd --variable=systemdsystemunitdir`"
else
    systemd_unit_dir="`$PKG_CONFIG systemd --variable=systemduserunitdir`"
fi

if test "x$systemd_unit_dir" = x; then
    AC_MSG_FAILURE([Cannot detect the systemd system unit dir])
fi
AC_SUBST(SYSTEMD_UNIT_DIR, $systemd_unit_dir)
AC_DEFINE([SYSTEMD_SUPPORT_ENABLED], [1], [Enable systemd support int the project])
AM_CONDITIONAL([SYSTEMD_SUPPORT_ENABLED], [test "x$systemd" = xyes ])

CXXFLAGS_DEBUG="-g -O2"
AC_ARG_ENABLE(
    [debug-build],
    [AS_HELP_STRING([--enable-debug-build],[enable debugging flags (default=no)])],
    [case "${enableval}" in
        yes) debug=yes ;;
        no) debug=no ;;
        *) AC_MSG_ERROR([bad value ${enableval} for --enable-debug-build]) ;;
    esac],
    [debug=no]
)

CXXFLAGS=" $EXTERNAL_CXXFLAGS"
CXXFLAGS="$CXXFLAGS -std=c++14"
CXXFLAGS="$CXXFLAGS -pedantic"
CXXFLAGS="$CXXFLAGS -Wno-unknown-pragmas"
CXXFLAGS="$CXXFLAGS -Wall"
CXXFLAGS="$CXXFLAGS -Wextra"

if test "x$debug" = xyes; then
    CXXFLAGS="$CXXFLAGS $CXXFLAGS_DEBUG"
fi

AC_SUBST(CXXFLAGS, $CXXFLAGS)
AC_SUBST(config_PATH, $prefix/.config)
AC_SUBST(NOTIFICATION_PATH, $notification_path)

AM_CONDITIONAL([NOTIFIER_CLI_ENABLED], [test "x$notifier_cli_enabled" = xyes ])
AM_CONDITIONAL([CUSTOM_USBGUARD_DEVEL_ENABLED], [test "x$custom_usbguard_devel_enabled" = "xyes"])

AC_CONFIG_FILES([
    Makefile
])

AC_OUTPUT

echo ""
echo "============== MACROS ================="
echo "          systemd: $systemd_unit_dir"
echo ""
echo "========== LINKER OPTIONS ============="
echo "        libnotify: $libnotify_summary"
echo "      libusbguard: $libusbguard_summary"
echo "          librsvg: $librsvg_summary"
echo "         CXXFLAGS: $CXXFLAGS"
echo ""
echo "============= ARGUMENTS ==============="
echo "           prefix: $prefix"
echo "      config_file: $prefix/.conf"
echo "notification_path: $notification_path"
echo "     notifier_cli: $notifier_cli_enabled"
echo ""
