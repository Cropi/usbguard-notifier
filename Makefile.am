CLEANFILES=
ACLOCAL_AMFLAGS= -I m4

bin_PROGRAMS = usbguard_notifier
usbguard_notifier_SOURCES=\
	src/Main.cpp \
	src/Log.cpp \
	src/Log.hpp \
	src/Notifier.hpp \
	src/Notifier.cpp

usbguard_notifier_LDFLAGS=\
	@notify_LIBS@ \
	@usbguard_LIBS@

usbguard_notifier_CPFLAGS=\
	-std=c++11 \
	@notify_CFLAGS@ \
	@usbguard_CFLAGS@

