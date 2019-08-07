#include "Notifier.hpp"

#include <usbguard/IPCClient.hpp>

int main()
{
    usbguardNotifier::Notifier notifier;

    notifier.start();
    while(1);
    return 0;
}

