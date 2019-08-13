#include "Notifier.hpp"

int main()
{
    usbguardNotifier::SignalWatcher n;

    n.connect();
    n.wait();

    return EXIT_SUCCESS;
}

