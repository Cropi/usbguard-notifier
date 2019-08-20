#include "Notifier.hpp"

#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include <usbguard/Exception.hpp>

static const char* short_options = "wh";

static const struct ::option long_options[] = {
    { "wait", no_argument, nullptr, 'w' },
    { "help", no_argument, nullptr, 'h' }
};

void showHelp(int argc, char** argv, std::ostream& output)
{
    output << "Usage: " << argv[0] << "[OPTIONS]" << std::endl;
    output << std::endl;
    output << "Option(s):" << std::endl;
    output << "    -w, --wait      Wait until an active IPC connection is estabilished." << std::endl;
    output << "    -h, --help      Show this usage message." << std::endl;
}

int main(int argc, char** argv)
{
    int opt;
    bool wait_connection = false;

    while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != EOF) {
        switch (opt) {
        case 'w':
            wait_connection = true;
            break;
        case 'h':
            showHelp(argc, argv, std::cerr);
            return EXIT_SUCCESS;
        case '?':
            showHelp(argc, argv, std::cerr);
            return EXIT_FAILURE;
        default:
            return EXIT_FAILURE;
        }
    }

    usbguardNotifier::Notifier notifier("usbguard-notifier");
    while (1) {
        try {
            notifier.connect();
            notifier.wait();
        } catch (const usbguard::Exception& e) {
            if (wait_connection) {
                continue;
            }

            std::cerr << "IPC connection failure!" << e.message() << std::endl;
            std::cerr << "Check if usbguard-daemon is running in the background" << std::endl;

            sleep(1);
        }
    }
    return EXIT_SUCCESS;
}

