/*
 * Copyright (C) 2019 Red Hat, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author(s): Attila Lakatos <alakatos@redhat.com>
 *            Zoltán Fridrich <zfridric@redhat.com>
 */

#include "BuildConfig.h"
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

    // Load notifications from file
    std::string notification_path = NOTIFICATION_FILE;
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
        std::getline(std::cin, cmd_options);
        cmd_options.erase(0, cmd_options.find_first_not_of(' '));
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
