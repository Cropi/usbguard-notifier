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
 */

#include "Log.hpp"
#include "Notifier.hpp"

#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <libgen.h>
#include <unistd.h>

#include <usbguard/Exception.hpp>

static const char* short_options = "wn:dh";

static const struct ::option long_options[] = {
    { "wait",  no_argument, nullptr, 'w' },
    { "num-attempts",  required_argument, nullptr, 'n' },
    { "debug", no_argument, nullptr, 'd' },
    { "help",  no_argument, nullptr, 'h' }
};

void showHelp(const std::string& app_name, std::ostream& out)
{
    out << "Usage: " << app_name << " [OPTIONS]" << std::endl;
    out << std::endl;
    out << "Options:" << std::endl;
    out << "    -w, --wait               Wait until an active IPC connection is estabilished." << std::endl;
    out << "    -n, --num-attempts <num> Number of IPC connection attempts." << std::endl;
    out << "    -d, --debug              Enable debug mode." << std::endl;
    out << "    -h, --help               Show this usage message." << std::endl;
}

int main(int argc, char** argv)
{
    const std::string app_name(::basename(*argv));
    bool wait_connection = false, debug = false;
    int opt, num_attempts = 3;

    while ((opt = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
        switch (opt) {
        case 'w':
            wait_connection = true;
            break;
        case 'n':
            num_attempts = std::atoi(optarg);
            break;
        case 'd':
            debug = true;
            break;
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
    NOTIFIER_LOGGER.setDebugMode(debug);

    bool print_err = true;
    for (int i = 0; wait_connection || i < num_attempts; ++i) {
        try {
            usbguardNotifier::Notifier notifier(app_name);
            notifier.connect();
            std::cout << "Connection has been established" << std::endl;
            print_err = true;
            i = 0;
            notifier.wait();
        } catch (const std::runtime_error& e) {
            std::cerr << "Error:" << e.what() << std::endl;
            return EXIT_FAILURE;
        } catch (const usbguard::Exception& e) {
            if (print_err) {
                print_err = false;
                std::cerr << "IPC connection failure!" << e.message() << std::endl;
                std::cerr << "Check if usbguard-daemon is running in the background" << std::endl;
            }
        }
        sleep(1);
    }
    return EXIT_SUCCESS;
}
