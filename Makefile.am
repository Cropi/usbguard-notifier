ACLOCAL_AMFLAGS = -I m4
CLEANFILES =

SUBDIRS = src/Tests/
bin_PROGRAMS = usbguard-notifier usbguard-notifier-cli

usbguard_notifier_SOURCES = \
	src/Notifier.hpp \
	src/NotifyWrapper.hpp \
	src/Serializer.hpp \
	src/Log.hpp \
	src/Main.cpp \
	src/Notifier.cpp \
	src/NotifyWrapper.cpp \
	src/Serializer.cpp \
	src/Log.cpp \
	icons/usbguard-icon.svg

usbguard_notifier_LDFLAGS = \
	@rsvg_LIBS@ \
	@notify_LIBS@ \
	@usbguard_LIBS@

usbguard_notifier_CXXFLAGS = \
	@rsvg_CFLAGS@ \
	@notify_CFLAGS@ \
	@usbguard_CFLAGS@

BUILT_SOURCES = \
	src/BuildConfig.h

usbguard_notifier_cli_SOURCES = \
	src/Serializer.hpp \
	src/NotifierCLI.hpp \
	src/Serializer.cpp \
	src/NotifierCLI.cpp \
	src/Common.hpp \
	src/MainCli.cpp

usbguard_notifier_cli_LDFLAGS = \
	@usbguard_LIBS@

usbguard_notifier_cli_CXXFLAGS = \
	@usbguard_CFLAGS@

#
# manual pages
#
SUFFIXES = .adoc

.adoc:
	$(A2X) -f manpage $^

man1_MANS = \
	$(top_builddir)/man/usbguard-notifier.1 \
	$(top_builddir)/man/usbguard-notifier-cli.1

CLEANFILES += \
	$(top_builddir)/man/usbguard-notifier.1 \
	$(top_builddir)/man/usbguard-notifier-cli.1

#
# usbguard icon
#
.svg.o:
	$(LD) -r -b binary -o $@ $<

#
# unit file
#
install-data-hook: install-systemd-service install-usbguard-notifier-conf
uninstall-hook: uninstall-systemd-service uninstall-usbguard-notifier-conf

CLEANFILES += $(top_builddir)/usbguard-notifier.service

#
# Notifier service
#
usbguard-notifier.service: $(top_srcdir)/usbguard-notifier.service.in
	$(SED) -e "s|%bindir%|${bindir}|" $^ > $@ || rm -f $@

install-systemd-service: $(top_builddir)/usbguard-notifier.service
	$(MKDIR_P) $(DESTDIR)$(SYSTEMD_UNIT_DIR)
	$(INSTALL) -m 644 $(top_builddir)/usbguard-notifier.service \
	$(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard-notifier.service

uninstall-systemd-service:
	rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard-notifier.service

CLEANFILES += $(top_builddir)/usbguard-notifier.service

#
# Saved notificaitons file path
#
.PHONY: usbguard-notifier.conf
usbguard-notifier.conf: $(top_builddir)/usbguard-notifier.conf.in
	$(MKDIR_P) @notification_PATH@
	if ! test -f "@notification_PATH@/notifications.log"; then \
		touch @notification_PATH@/notifications.log && \
		chmod 666 @notification_PATH@/notifications.log; \
	fi
	$(SED) \
		-e "s|%notification_path%|@notification_PATH@|g" \
		$^ > $@ || rm -f $@

install-usbguard-notifier-conf: $(top_builddir)/usbguard-notifier.conf
	$(MKDIR_P) @config_PATH@/usbguard-notifier
	$(INSTALL) -m 666 \
		$(top_builddir)/usbguard-notifier.conf \
		@config_PATH@/usbguard-notifier/usbguard-notifier.conf

uninstall-usbguard-notifier-conf:
	rm -rf @config_PATH@/usbguard-notifier

CLEANFILES += $(top_builddir)/usbguard-notifier.conf

#
# Common defines
#
$(top_builddir)/src/BuildConfig.h: $(top_builddir)/src/BuildConfig.h.in
	$(SED) -e "s|%CONF_FILE%|@config_PATH@/usbguard-notifier/usbguard-notifier.conf|g" $^ > $@ || rm -f $@

CLEANFILES += $(top_builddir)/src/BuildConfig.h
