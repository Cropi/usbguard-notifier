#include "Notifier.hpp"

#include <usbguard/Exception.hpp>

#include <iostream>
#include <getopt.h>
#include <unistd.h>

static const char* short_options = "wh";

static const struct ::option long_options[] = {
    { "wait", no_argument, nullptr, 'w' },
    { "help", no_argument, nullptr, 'h' }
};

void showHelp(const std::string& appName, std::ostream& output)
{
    output << "Usage: " << appName << " [OPTIONS]" << std::endl;
    output << std::endl;
    output << "Options:" << std::endl;
    output << "    -w, --wait      Wait until an active IPC connection is estabilished." << std::endl;
    output << "    -h, --help      Show this usage message." << std::endl;
}

int main(int argc, char** argv)
{
    const std::string appName(*argv);
    bool waitConnection = false;
    int opt;

    while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != EOF) {
        switch (opt) {
        case 'w':
            waitConnection = true;
            break;
        case 'h':
            showHelp(appName, std::cout);
            return EXIT_SUCCESS;
        case '?':
            showHelp(appName, std::cerr);
            return EXIT_FAILURE;
        default:
            return EXIT_FAILURE;
        }
    }
    usbguardNotifier::Notifier notifier(appName);

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

