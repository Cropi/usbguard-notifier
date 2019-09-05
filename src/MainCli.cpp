#include "BuildConfig.h"
#include "Common.hpp"
#include "NotifierCLI.hpp"

#include <usbguard/ConfigFile.hpp>
#include <usbguard/Exception.hpp>

#include <iostream>
#include <getopt.h>
#include <libgen.h>
#include <unistd.h>

static const char* shortOptions = "h";

static const struct ::option longOptions[] = {
    { "help",  no_argument, nullptr, 'h' }
};

void showHelp(const std::string& app_name, std::ostream& out)
{
    out << "Usage: " << app_name << " [OPTIONS]" << std::endl;
    out << std::endl;
    out << "Options:" << std::endl;
    out << "    -h, --help      Show this usage message." << std::endl;
}

int main(int argc, char** argv)
{
    using namespace usbguardNotifier;
    const std::string app_name(::basename(*argv));
    int opt;

    while ((opt = getopt_long(argc, argv, shortOptions, longOptions, nullptr)) != -1) {
        switch (opt) {
        case 'h':
            showHelp(app_name, std::cout);
            return EXIT_SUCCESS;
        case '?':
            showHelp(app_name, std::cerr);
            return EXIT_FAILURE;
        default:
            return EXIT_FAILURE;
        }
    }

    usbguard::ConfigFile config(g_nconfig_names);
    try {
        config.open(CONF_FILE, /*readonly=*/true);
    } catch (usbguard::Exception&) {
        std::cerr <<  "Error: Could not open the configuration file." << std::endl;
        return EXIT_FAILURE;
    }

    Serializer serializer(config.getSettingValue("NotificationPath"));
    std::map<unsigned, Notification> notifications = serializer.deserializeAll();

    NotifierCLI::Methods state = NotifierCLI::Methods::CLI_SHOW;
    std::string line;
    std::string command_key, command_options;

    NotifierCLI notifier(notifications);
    while (state != NotifierCLI::Methods::CLI_QUIT) {
        std::cin >> line;
        command_key = line.substr(0, line.find(" "));
        command_options = line.substr(line.find(""));

        try {
            state = notifier.execute(command_key, command_options);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return EXIT_SUCCESS;
}

