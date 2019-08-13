CLEANFILES=
ACLOCAL_AMFLAGS= -I m4

sbin_PROGRAMS = usbguard_notifier
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

install-data-hook: install-systemd-service
uninstall-hook: uninstall-systemd-service

usbguard_notifier.service: $(top_srcdir)/usbguard_notifier.service.in
		$(SED) -e "s|%sbindir%|${sbindir}|" -e "s|%sysconfdir%|${sysconfdir}|" $^ > $@ || rm -f $@

CLEANFILES+=usbguard_notifier.service

install-systemd-service: $(top_builddir)/usbguard_notifier.service
		$(MKDIR_P) $(DESTDIR)$(SYSTEMD_UNIT_DIR)
		$(INSTALL) -m 644 $(top_builddir)/usbguard_notifier.service \
		$(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard_notifier.service

uninstall-systemd-service:
		rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard_notifier.service
