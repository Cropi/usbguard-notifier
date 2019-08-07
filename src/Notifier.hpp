#include "IPCMessage.hpp"

#include <qb/qbipcc.h>
#include <qb/qbloop.h>

#include <iostream>
#include <map>

namespace usbguardNotifier
{
    class Notifier;
    using MessageObserver = IPC::MessageObserver<Notifier>;

    class Notifier
    {
    public:
        Notifier();
        ~Notifier();
        void start();
        void stop();
        std::map<uint32_t, MessageObserver> getObservers();

        qb_ipcc_connection_t* qbGetCon();

    private:

        template<class T>
        void createObserver(const uint32_t observer_type, MessageObserver::methodType method)
        {
            // TODO
            _observers.emplace(observer_type, MessageObserver::create<T>(*this, method));
        }

        void handleDevicePolicyChanged(IPC::MessagePointer& request, IPC::MessagePointer& response);
        void handleDeviceAvailabilityChanged(IPC::MessagePointer& request, IPC::MessagePointer& response);

        qb_ipcc_connection_t* _qb_conn;
        qb_loop_t* _qb_loop;
        int _fd;
        std::map<uint32_t, MessageObserver> _observers;
    };

    class Event
    {
    public:
        Event(Notifier* notifier);
        void process();
    private:
        std::string rcvMessage();

        Notifier* _notifier;
    };

#define QB_MS_TIMEOUT (500)
#define QB_BUF_SIZE (1 << 20)
} /* namespace usbguardNotifier */
