#include "NotifierCLI.hpp"

#include <iostream>
#include <iterator>

namespace usbguardNotifier
{

struct command_data {
        NotifierCLI::Methods command_type;
        void(NotifierCLI::*command)(std::string);
};

static const std::map<std::string, command_data> cmd_handler_map = {
    { "show", { NotifierCLI::Methods::CLI_SHOW, &NotifierCLI::show }},
    { "display", { NotifierCLI::Methods::DISPLAY, &NotifierCLI::display }},
    { "jump", { NotifierCLI::Methods::JUMP, &NotifierCLI::jump }},
    { "next", { NotifierCLI::Methods::NEXT, &NotifierCLI::next }},
    { "previous", { NotifierCLI::Methods::PREVIOUS, &NotifierCLI::previous }},
    { "remove", { NotifierCLI::Methods::REMOVE, &NotifierCLI::remove }},
    { "help", { NotifierCLI::Methods::HELP, &NotifierCLI::help }},
    { "commands", { NotifierCLI::Methods::COMMANDS, &NotifierCLI::commands }},
    { "quit", { NotifierCLI::Methods::CLI_QUIT, &NotifierCLI::quit }}
};

NotifierCLI::NotifierCLI(std::map<unsigned, Notification> notifications)
    : _notifications(std::move(notifications)),
    _iter(_notifications.end()) {}

NotifierCLI::Methods NotifierCLI::execute(
        std::string& command_key,
        std::string& command_options)
{
    const auto iterator = cmd_handler_map.find(command_key);
    if (iterator == cmd_handler_map.cend()) {
        return NotifierCLI::Methods::UNKNOWN;
    }
    //auto command = iterator->second.command;
    (*this.*(iterator->second.command))(command_options);
    return iterator->second.command_type;
}

void NotifierCLI::show(std::string /*options*/)
{
}

void NotifierCLI::display(std::string /*options*/)
{
    std::cout << "Notification #" << _iter->first << ":\n";
    std::cout << "    Name:      " << _iter->second.device_name << '\n';
    std::cout << "    Type:      " << _iter->second.event_type << '\n';
    std::cout << "    Old value: " << _iter->second.target_old << '\n';
    std::cout << "    New value: " << _iter->second.target_new << '\n';
    std::cout << "    Rule:      " << _iter->second.rule << std::endl;
}

void NotifierCLI::jump(std::string options)
{
    try {
        auto i = _notifications.find(static_cast<unsigned>(std::stoul(options)));
        if (i == _notifications.end()) {
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
void NotifierCLI::next(std::string /*options*/)
{
    if (_iter == _notifications.end()) {
        std::cout << "At EOF." << std::endl;
        return;
    }
    std::next(_iter);
}

// TODO change
void NotifierCLI::previous(std::string /*options*/)
{
 if (_iter == _notifications.end() || _iter == _notifications.begin()) {
        std::cout << "At EOF." << std::endl;
        return;
    }
    std::prev(_iter);
}

// TODO change
void NotifierCLI::remove(std::string /*options*/)
{
}

void NotifierCLI::help(std::string /*options*/)
{
}

void NotifierCLI::commands(std::string /*options*/)
{
    for (const auto &i : cmd_handler_map) {
        std::cout << i.first << std::endl;
    }
}

void NotifierCLI::quit(std::string /*options*/)
{
}

}
