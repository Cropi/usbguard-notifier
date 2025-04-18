##
## Copyright (C) 2019 Red Hat, Inc.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 2 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <https://www.gnu.org/licenses/>.
##
## Author(s): Zoltán Fridrich <zfridric@redhat.com>
##  

AM_CPPFLAGS = \
    -I$(top_srcdir)/src \
    @catch_CFLAGS@ \
	-fPIC

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
    Unit/test_notifier_cli.cpp \
    ../NotifierCLI.hpp \
    ../NotifierCLI.cpp \
    ../Serializer.hpp \
    ../Serializer.cpp

test_unit_LDADD = \
    $(catch_LIBS)
