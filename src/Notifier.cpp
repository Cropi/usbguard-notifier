#include "Notifier.hpp"
#include "Log.hpp"

#include <string>
#include <iostream>

namespace usbguardNotifier
{
    Notifier::Notifier()
    {
        _qb_loop = qb_loop_create();

        createObserver<IPC::DevicePresenceChangedSignal>(3, &Notifier::handleDeviceAvailabilityChanged);
        createObserver<IPC::DevicePolicyChangedSignal>(4, &Notifier::handleDevicePolicyChanged);
    }

    void Notifier::handleDevicePolicyChanged(IPC::MessagePointer& request, IPC::MessagePointer& response)
    {
        NOTIFIER_LOG() << "Handle device policy changed signal";

        const IPC::DevicePolicyChangedSignal* const signal = \
            reinterpret_cast<const IPC::DevicePolicyChangedSignal*>(request.get());

        NOTIFIER_LOG() << \
            "id=" << signal->id() << "  " << \
            "device_rule=" << signal->device_rule() << "  " << \
            "rule_id=" << signal->rule_id() << " " \
            "target_old=" << IPC::targetToStr(signal->target_old()) << " " \
            "target_new=" << IPC::targetToStr(signal->target_new());

        // TODO
    }

    void Notifier::handleDeviceAvailabilityChanged(IPC::MessagePointer& request, IPC::MessagePointer& response)
    {
        NOTIFIER_LOG() << "Handle device availability changed signal";

        const IPC::DevicePresenceChangedSignal* const signal = \
            reinterpret_cast<const IPC::DevicePresenceChangedSignal*>(request.get());

        NOTIFIER_LOG() << \
            "id=" << signal->id() << " " << \
            "event=" << signal->event() << " " << \
            "target=" << IPC::targetToStr(signal->target()) << " " \
            "device_rule" << signal->device_rule();

        // TODO
    }

    Notifier::~Notifier()
    {
        stop();
    }

    int32_t qbIPCEventHappened(int32_t fd, int32_t revents, void *data)
    {
        NOTIFIER_LOG() << "An event has occured";

        Notifier* notifier = static_cast<Notifier*>(data);
        Event event(notifier);
        event.process();

        return 0;
    }

    void Notifier::start()
    {
        NOTIFIER_LOG() << "Start IPC connection";

        if ((_qb_conn = qb_ipcc_connect("usbguard", QB_BUF_SIZE)) == NULL) {
            // TODO throw error
            std::cerr << "ipc error\n";
        }

        qb_ipcc_fd_get(_qb_conn, &_fd);
        if (_fd < 0) {
            // TODO throw error
            std::cerr << "fd_get error\n";
        }

        qb_loop_poll_add(_qb_loop, QB_LOOP_HIGH, _fd, POLLIN, this, qbIPCEventHappened);
        qb_loop_run(_qb_loop);
    }

    void Notifier::stop()
    {
        NOTIFIER_LOG() << "Stop IPC connection";
    }

    qb_ipcc_connection_t* Notifier::qbGetCon()
    {
        return _qb_conn;
    }

    std::map<uint32_t, MessageObserver> Notifier::getObservers()
    {
        return _observers;
    }

    Event::Event(Notifier *notifier)
        : _notifier(notifier)
    {
    }

    void Event::process()
    {
        NOTIFIER_LOG() << "Processing the message";
        std::string buffer = rcvMessage();

        const struct qb_ipc_response_header* header = reinterpret_cast<const struct qb_ipc_response_header*>(buffer.data());

        if (header->size != (int32_t)buffer.size()) {
            // TODO
        }

        const int32_t request_type = header->id - QB_IPC_MSG_USER_START;
        const std::string payload = buffer.substr(sizeof(struct qb_ipc_response_header));
        NOTIFIER_LOG() << "Request type: " << request_type << "Message: " << payload;

        IPC::MessagePointer response;
        auto& observer = _notifier->getObservers().at(request_type);
        IPC::MessagePointer request = observer.stringToMessagePointer(payload);
        observer.run(request, response);
    }

    std::string Event::rcvMessage()
    {
        NOTIFIER_LOG() << "Receive message";

        std::string buffer(QB_BUF_SIZE, '\0');
        ssize_t msg_size;

        msg_size = qb_ipcc_event_recv(
            /*c=*/ _notifier->qbGetCon(),
            /*msg_ptr=*/ &buffer[0],
            /*msg_len=*/ QB_BUF_SIZE,
            /*ms_timeout=*/ QB_MS_TIMEOUT);
        if (msg_size < 0) {
            //TODO
            std::cerr << "msgsize error";
        }

        buffer.resize(msg_size);

        return buffer;
    }
} /* namespace usbguardNotifier */






















/**/
