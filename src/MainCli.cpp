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
    CLI notifier(serializer.deserializeAll());

    CLI::Command cmd_code = CLI::Command::UNKNOWN;
    std::string line, cmd_name, cmd_options;

    while (cmd_code != CLI::Command::QUIT) {
        std::cin >> cmd_name;
        // TODO figure out how to eliminate whitespace
        std::getline(std::cin, cmd_options);

        // std::cout << "|" << cmd_name << "| |" << cmd_options << "|\n";
        try {
            cmd_code = notifier.execute(cmd_name, cmd_options);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return EXIT_SUCCESS;
}
