#include "NotifierCLI.hpp"

namespace usbguardNotifier
{

using methodyType = NotifierCLI::Methods(NotifierCLI::*)(std::string options);

static const std::map<std::string, methodyType> cmd_handler_map = {
    { "show", &NotifierCLI::show },
    { "quit", &NotifierCLI::quit }
};

NotifierCLI::NotifierCLI(std::map<unsigned, Notification> notifications)
    : _notifications(notifications) {}

NotifierCLI::Methods NotifierCLI::execute(std::string& command_key, std::string& command_options)
{
    auto iterator(cmd_handler_map.find(command_key));

    if (iterator == cmd_handler_map.cend()) {
        throw std::runtime_error("Unrecognized command");
    }

    auto command = iterator->second;

    return (*this.*command)(command_options);
}

NotifierCLI::Methods NotifierCLI::show(std::string /*options*/)
{
    return Methods::CLI_SHOW;
}

NotifierCLI::Methods NotifierCLI::quit(std::string /*options*/)
{
    return Methods::CLI_QUIT;
}
}
