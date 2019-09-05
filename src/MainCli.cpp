#include "NotifierCLI.hpp"
#include "build-config.h"

#include <usbguard/ConfigFile.hpp>

#include <iostream>
#include <getopt.h>
#include <libgen.h>
#include <unistd.h>

static const char* shortOptions = "h";

static const struct ::option longOptions[] = {
    { "help",  no_argument, nullptr, 'h' }
};

void showHelp(const std::string& appName, std::ostream& out)
{
    out << "Usage: " << appName << " [OPTIONS]" << std::endl;
    out << std::endl;
    out << "Options:" << std::endl;
    out << "    -h, --help      Show this usage message." << std::endl;
}

static std::vector<std::string> config_names = {
    "NotificationPath"
};

int main(int argc, char** argv)
{
    using namespace usbguardNotifier;
    const std::string appName(::basename(*argv));
    int opt;

    while ((opt = getopt_long(argc, argv, shortOptions, longOptions, nullptr)) != -1) {
        switch (opt) {
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

    usbguard::ConfigFile config(config_names);
    config.open(CONF_FILE, /*readonly=*/true);

    Serializer serializer(/*file_name=*/config.getSettingValue("NotificationPath"));
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

