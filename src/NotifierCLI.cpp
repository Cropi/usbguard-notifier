#include "NotifierCLI.hpp"

#include <iostream>
#include <iterator>
#include <vector>

namespace usbguardNotifier
{

struct cmd_data {
        CLI::Command code;
        void(CLI::*command)(const std::string&);
        std::string description;
};

static const std::map<std::string, cmd_data> commands = {
    { "show", { CLI::Command::SHOW, &CLI::show,
        "Show all notifications." }},
    { "display", { CLI::Command::DISPLAY, &CLI::display,
        "Display detailed description of current notification." }},
    { "jump", { CLI::Command::JUMP, &CLI::jump,
        "Jump to the specified index." }},
    { "next", { CLI::Command::NEXT, &CLI::next,
        "Move cursor to next notification." }},
    { "previous", { CLI::Command::PREVIOUS, &CLI::previous,
        "Move cursor to previous notification." }},
    { "help", { CLI::Command::HELP, &CLI::help,
        "Show this help." }},
    { "list", { CLI::Command::LIST, &CLI::list,
        "List all available commands." }},
    { "quit", { CLI::Command::QUIT, &CLI::quit,
        "Quit CLI." }}
};

CLI::CLI(const std::map<unsigned, Notification>& notifications)
    : _db(notifications),
    _iter(_db.cbegin()) {}

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
        std::cout << (n.first == _iter->first ? "> " : "  ")
            << "#" << n.first << ": " << n.second.event_type << " <"
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
    try {
        auto i = _db.find(static_cast<unsigned>(std::stoul(options)));
        if (i == _db.cend()) {
            std::cerr << "There is no element with such index." << std::endl;
            return;
        }
        _iter = i;
    } catch (std::logic_error &e) {
        std::cerr << "Invalid index passed to jump." << std::endl;
        return;
    }
}

void CLI::next(const std::string& /*options*/)
{
    if (_db.empty() || _iter == std::prev(_db.cend())) {
        std::cout << "At EOF." << std::endl;
        return;
    }
    _iter = std::next(_iter);
}

void CLI::previous(const std::string& /*options*/)
{
    if (_db.empty() || _iter == _db.cbegin()) {
        return;
    }
    _iter = std::prev(_iter);
}

void CLI::help(const std::string& /*options*/)
{
    for (const auto& i : commands) {
        std::cout << i.first << " - " << i.second.description << std::endl;
    }
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
