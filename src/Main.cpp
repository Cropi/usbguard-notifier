#include "Notifier.hpp"
#include "Log.hpp"

#include <usbguard/Exception.hpp>

#include <iostream>
#include <libgen.h>
#include <getopt.h>
#include <unistd.h>

static const char* shortOptions = "wdh";

static const struct ::option longOptions[] = {
    { "wait",  no_argument, nullptr, 'w' },
    { "debug", no_argument, nullptr, 'd' },
    { "help",  no_argument, nullptr, 'h' }
};

void showHelp(const std::string& appName, std::ostream& out)
{
    out<< "Usage: " << appName << " [OPTIONS]" << std::endl;
    out<< std::endl;
    out<< "Options:" << std::endl;
    out<< "    -w, --wait      Wait until an active IPC connection is estabilished." << std::endl;
    out<< "    -d, --debug     Enable debug mode." << std::endl;
    out<< "    -h, --help      Show this usage message." << std::endl;
}

int main(int argc, char** argv)
{
    const std::string appName(::basename(*argv));
    bool waitConnection = false, debug = false;
    int opt;

    while ((opt = getopt_long(argc, argv, shortOptions, longOptions, nullptr)) != -1) {
        switch (opt) {
        case 'w':
            waitConnection = true;
            break;
        case 'h':
            showHelp(appName, std::cout);
            return EXIT_SUCCESS;
        case 'd':
            debug = true;
            break;
        case '?':
            showHelp(appName, std::cerr);
            return EXIT_FAILURE;
        default:
            return EXIT_FAILURE;
        }
    }
    usbguardNotifier::Notifier notifier(appName);
    NOTIFIER_LOGGER.setDebugMode(debug);

    for (;;) {
        try {
            notifier.connect();
            notifier.wait();
        } catch (const usbguard::Exception& e) {
            if (waitConnection) {
                continue;
            }
            std::cerr << "IPC connection failure!" << e.message() << std::endl;
            std::cerr << "Check if usbguard-daemon is running in the background" << std::endl;

            sleep(1);
        }
    }
    return EXIT_SUCCESS;
}
