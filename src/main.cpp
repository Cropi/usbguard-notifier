#include "Notifier.hpp"

int main()
{
    usbguardNotifier::Notifier notifier;

    notifier.start();
    while(1);
    return 0;
}

