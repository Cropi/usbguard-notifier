#ifndef NOTIFIER_CLI_HPP
#define NOTIFIER_CLI_HPP

#include "Serializer.hpp"
#include "Notifier.hpp"

#include <map>
#include <string>

namespace usbguardNotifier
{

class NotifierCLI
{
public:

    enum class Methods {
        CLI_SHOW,
        CLI_QUIT
    };

    /**
     * @brief Constructs NotifierCLI
     *
     * @param notifications map of notifications
     */
    NotifierCLI(std::map<unsigned, Notification> notifications);

    /**
     * @brief Executes the given command
     *
     * @param command_key the command to be executed
     * @param command_options options need for the command
     *
     * @return command code
     */
    Methods execute(std::string& command_key, std::string& command_options);

    /**
     * @brief Show notifications
     *
     * @param options are used to select only certain notifications
     *
     * @return command code
     */
    Methods show(std::string options);

    /**
     * @brief Show notifications
     *
     * @param options are used to select only certain notifications
     *
     * @return command code
     */
    Methods quit(std::string options);

private:
    std::map<unsigned, Notification> _notifications;
};
} /* namespace usbguardNotifier */

#endif
