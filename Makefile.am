bin_PROGRAMS = usbguard_notifier
usbguard_notifier_SOURCES=\
	src/main.cpp \
	src/Log.cpp \
	src/Log.hpp \
	src/Notifier.hpp \
	src/Notifier.cpp \
	src/IPCMessage.hpp \
	src/IPCMessage.cpp \
	$(proto_FILES)

proto_FILES=\
	src/Library/IPC/Devices.pb.h \
	src/Library/IPC/Devices.pb.cc

src/Library/IPC/%.pb.cc src/Library/IPC/%.pb.h: src/Library/IPC/%.proto
	$(PROTOC) --cpp_out=. $^

ACLOCAL_AMFLAGS= -I m4

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
	-lprotoc\
	-lqb\
	-ldl

