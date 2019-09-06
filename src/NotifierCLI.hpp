#ifndef NOTIFIER_CLI_HPP
#define NOTIFIER_CLI_HPP

#include "Serializer.hpp"

#include <map>
#include <string>

namespace usbguardNotifier
{

class NotifierCLI
{
public:

    enum class Methods {
        UNKNOWN,
        CLI_SHOW, // TODO rename
        DISPLAY,
        JUMP,
        NEXT,
        PREVIOUS,
        REMOVE,
        HELP,
        COMMANDS,
        CLI_QUIT // TODO rename
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
    
    void show(std::string options);
    void display(std::string options);
    void jump(std::string options);
    void next(std::string options);
    void previous(std::string options);
    void remove(std::string options);
    void help(std::string options);
    void commands(std::string options);
    void quit(std::string options);


private:

    // TODO dont execute anything if db is empty
    // TODO maybe store as a reference
    std::map<unsigned, Notification> _notifications;

    std::map<unsigned, Notification>::iterator _iter;
};

} // namespace usbguardNotifier

#endif // NOTIFIER_CLI_HPP
