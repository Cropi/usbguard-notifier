CLEANFILES=
ACLOCAL_AMFLAGS=

bin_PROGRAMS = usbguard_notifier
usbguard_notifier_SOURCES=\
	$(proto_FILES) \
	src/main.cpp \
	src/Log.cpp \
	src/Log.hpp \
	src/Notifier.hpp \
	src/Notifier.cpp \
	src/IPCMessage.hpp \
	src/IPCMessage.cpp

proto_FILES=\
	src/Library/IPC/Devices.pb.h \
	src/Library/IPC/Devices.pb.cc

CLEANFILES+=$(proto_FILES)

%.pb.cc %.pb.h: %.proto
	$(PROTOC) --cpp_out=. $^

ACLOCAL_AMFLAGS+= -I m4

usbguard_notifier_LDFLAGS=\
	-lprotobuf \
	-lpthread \
	-lqb \
	-ldl

usbguard_notifier_CPFLAGS=\
	-std=c++11 \
	-Wall\
	-pedantic\
	-Werror\
	-pthread\
	-lprotoc
