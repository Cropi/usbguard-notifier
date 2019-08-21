CLEANFILES=
ACLOCAL_AMFLAGS= -I m4



bin_PROGRAMS = usbguard_notifier
usbguard_notifier_SOURCES=\
	src/Main.cpp \
	src/Log.cpp \
	src/Notifier.cpp \
  icons/usbguard-icon.svg

usbguard_notifier_LDFLAGS=\
	@notify_LIBS@ \
	@usbguard_LIBS@ \
  -lrsvg-2 -lm -lgio-2.0 -lgdk_pixbuf-2.0 -lgobject-2.0 -lglib-2.0 -lcairo

usbguard_notifier_CXXFLAGS=\
	@notify_CFLAGS@ \
	@usbguard_CFLAGS@

usbguard_notifier_CPPFLAGS=\
	-I/usr/include/librsvg-2.0/librsvg \
	-I/usr/include/cairo

#DEFS+=-DICON_PATH=\"file://$(datadir)/usbguard-notifier/usbguard-icon.svg\"

#appicondir=$(datadir)/usbguard-notifier
#appicon_DATA=icons/usbguard-icon.svg

install-data-hook: install-systemd-service
uninstall-hook: uninstall-systemd-service

usbguard_notifier.service: $(top_srcdir)/usbguard_notifier.service.in
		$(SED) -e "s|%bindir%|${sbindir}|" $^ > $@ || rm -f $@

CLEANFILES+=usbguard_notifier.service

install-systemd-service: $(top_builddir)/usbguard_notifier.service
		$(MKDIR_P) $(DESTDIR)$(SYSTEMD_UNIT_DIR)
		$(INSTALL) -m 644 $(top_builddir)/usbguard_notifier.service \
		$(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard_notifier.service

uninstall-systemd-service:
		rm -f $(DESTDIR)$(SYSTEMD_UNIT_DIR)/usbguard_notifier.service


.svg.o:
	$(LD) -r -b binary -o $@ $<
