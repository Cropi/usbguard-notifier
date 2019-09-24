AM_CPPFLAGS = \
    -I$(top_srcdir)/src \
    @catch_CFLAGS@

EXTRA_DIST = \
    $(top_srcdir)/src/ThirdParty/Catch2

TESTS_ENVIRONMENT = \
    builddir=$(top_builddir) \
    srcdir=$(top_srcdir) \
    abs_top_srcdir=$(abs_top_srcdir)

TESTS = \
    test-unit

check_PROGRAMS = \
    test-unit

test_unit_SOURCES=\
    main.cpp \
    Unit/test_example.cpp
