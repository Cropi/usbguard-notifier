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

    // Read config file
    usbguard::ConfigFile config(config_names);
    config.open(CONF_FILE, /*readonly=*/true);

    // Load notifications from file
    std::string notification_path = config.getSettingValue("NotificationPath");
    Serializer serializer(notification_path);
    std::map<unsigned, Notification> map;
    try {
        map = serializer.deserializeAll();
    } catch (std::runtime_error& e) {
        std::cerr << "Error: file " << notification_path
            << " doesnt exist." << std::endl;
        return EXIT_FAILURE;
    }
    if (map.empty()) {
        std::cout << "No notifications to show." << std::endl;
        return EXIT_SUCCESS;
    }
    CLI notifier(map);

    CLI::Command cmd_code = CLI::Command::UNKNOWN;
    std::string line, cmd_name, cmd_options;

    while (cmd_code != CLI::Command::QUIT) {
        std::cout << "& ";

        std::cin >> cmd_name;
        // TODO figure out how to eliminate whitespace
        std::getline(std::cin, cmd_options);

        // std::cout << "|" << cmd_name << "| |" << cmd_options << "|\n";
        try {
            cmd_code = notifier.execute(cmd_name, cmd_options);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
        if (cmd_code == CLI::Command::UNKNOWN) {
            std::cerr << "Unknown command." << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
