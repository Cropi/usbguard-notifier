ACLOCAL_AMFLAGS = -I m4
CLEANFILES =

bin_PROGRAMS = usbguard-notifier

usbguard_notifier_SOURCES = \
	src/Notifier.hpp \
	src/NotifyWrapper.hpp \
	src/Log.hpp \
	src/Main.cpp \
	src/Notifier.cpp \
	src/Log.cpp

usbguard_notifier_LDFLAGS = \
	@notify_LIBS@ \
	@usbguard_LIBS@

usbguard_notifier_CXXFLAGS = \
	@notify_CFLAGS@ \
	@usbguard_CFLAGS@

# manual pages

SUFFIXES = .adoc

.adoc: 
	$(A2X) -f manpage $^

man1_MANS = \
	$(top_builddir)/man/usbguard-notifier.1 \
	$(top_builddir)/man/usbguard-notifier-cli.1

# usbguard icon

DEFS += -DICON_PATH=\"file://$(datadir)/usbguard-notifier/usbguard-icon.svg\"

appicondir = $(datadir)/usbguard-notifier
appicon_DATA = icons/usbguard-icon.svg

# unit file

install-data-hook: install-systemd-service
uninstall-hook: uninstall-systemd-service

CLEANFILES += $(top_builddir)/usbguard-notifier.service

usbguard-notifier.service: $(top_srcdir)/usbguard-notifier.service.in
		$(SED) -e "s|%bindir%|${bindir}|" $^ > $@ || rm -f $@

install-systemd-service: $(top_builddir)/usbguard-notifier.service
		$(MKDIR_P) $(DESTDIR)$(SYSTEMD_UNIT_DIR)
		$(INSTALL) -m 644 $(top_builddir)/usbguard-notifier.service \
		$(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard-notifier.service

uninstall-systemd-service:
		rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard-notifier.service
