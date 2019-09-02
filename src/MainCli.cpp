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

int main(int argc, char** argv)
{
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
    return EXIT_SUCCESS;
}

