#include <qb/qbipcc.h>
#include <qb/qbloop.h>

#include <iostream>

namespace usbguardNotifier
{
    class Notifier
    {
    public:
        Notifier();
        ~Notifier();
        void start();
        void stop();

        qb_ipcc_connection_t* qbGetCon();
    private:

        qb_ipcc_connection_t* _qb_conn;
        qb_loop_t* _qb_loop;
        int _fd;
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
