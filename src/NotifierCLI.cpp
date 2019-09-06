#include "NotifierCLI.hpp"

#include <iostream>
#include <iterator>
#include <vector>

namespace usbguardNotifier
{

struct cmd_data {
        CLI::Command code;
        void(CLI::*command)(const std::string&);
};

static const std::map<std::string, cmd_data> commands = {
    { "show", { CLI::Command::SHOW, &CLI::show }},
    { "display", { CLI::Command::DISPLAY, &CLI::display }},
    { "jump", { CLI::Command::JUMP, &CLI::jump }},
    { "next", { CLI::Command::NEXT, &CLI::next }},
    { "previous", { CLI::Command::PREVIOUS, &CLI::previous }},
    { "remove", { CLI::Command::REMOVE, &CLI::remove }},
    { "help", { CLI::Command::HELP, &CLI::help }},
    { "list", { CLI::Command::LIST, &CLI::list }},
    { "quit", { CLI::Command::QUIT, &CLI::quit }}
};

// TODO might break on empty db
CLI::CLI(std::map<unsigned, Notification> notifications)
    : _db(std::move(notifications)),
    _iter(_db.begin()) {}

CLI::Command CLI::execute(
        const std::string& cmd_name,
        const std::string& cmd_options)
{
    const auto iterator = commands.find(cmd_name);
    if (iterator == commands.cend()) {
        return CLI::Command::UNKNOWN;
    }
    auto command = iterator->second.command;
    (*this.*command)(cmd_options);
    return iterator->second.code;
}

void CLI::show(const std::string& /*options*/)
{
    // TODO implement options

    for (const auto& n : _db) {
        if (n.first == _iter->first) {
            std::cout << "> ";
        } else {
            std::cout << "  ";
        }
        std::cout << "#" << n.first << ": " << n.second.event_type << " <"
            << n.second.device_name << "> " << n.second.target_new << std::endl;
    }
}

void CLI::display(const std::string& /*options*/)
{
    std::cout << "Notification #" << _iter->first << ":\n";
    std::cout << "    Name:      " << _iter->second.device_name << '\n';
    std::cout << "    Type:      " << _iter->second.event_type << '\n';
    std::cout << "    Old value: " << _iter->second.target_old << '\n';
    std::cout << "    New value: " << _iter->second.target_new << '\n';
    std::cout << "    Rule:      " << _iter->second.rule << std::endl;
}

void CLI::jump(const std::string &options)
{
    std::cout << "debug: |" << options << "|" << std::endl;
    try {
        auto i = _db.find(static_cast<unsigned>(std::stoul(options)));
        if (i == _db.end()) {
            std::cerr << "There is no element with such index." << std::endl;
            return;
        }
        _iter = i;
    } catch (std::logic_error &e) {
        std::cerr << e.what() << std::endl;
        return;
    }
}

// TODO change
void CLI::next(const std::string& /*options*/)
{

    if (_iter == _db.end()) {
        std::cout << "At EOF." << std::endl;
        return;
    }
    _iter = std::next(_iter);
}

// TODO change
void CLI::previous(const std::string& /*options*/)
{

    if (_iter == _db.end() || _iter == _db.begin()) {
        std::cout << "At EOF." << std::endl;
        return;
    }
    _iter = std::prev(_iter);
}

// TODO change
void CLI::remove(const std::string& /*options*/)
{
}

void CLI::help(const std::string& /*options*/)
{
}

void CLI::list(const std::string& /*options*/)
{
    for (const auto& i : commands) {
        std::cout << i.first << std::endl;
    }
}

void CLI::quit(const std::string& /*options*/)
{
}

}
