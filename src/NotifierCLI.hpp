#ifndef NOTIFIER_CLI_HPP
#define NOTIFIER_CLI_HPP

#include "Serializer.hpp"

#include <map>
#include <string>

namespace usbguardNotifier
{

class CLI
{
public:

    enum class Command {
        UNKNOWN,
        SHOW,
        DISPLAY,
        JUMP,
        NEXT,
        PREVIOUS,
        REMOVE,
        HELP,
        LIST,
        QUIT
    };

    /**
     * @brief Constructs usbguard-notifier CLI
     *
     * @param notifications Map of notifications
     */
    explicit CLI(std::map<unsigned, Notification> notifications);

    /**
     * @brief Parse and execute given command.
     *
     * @param cmd_name Name of the command.
     * @param cmd_options Options passed to the command.
     *
     * @return Command code.
     */
    Command execute(
            const std::string& cmd_name,
            const std::string& cmd_options);

    void show(const std::string& options);
    void display(const std::string& options);
    void jump(const std::string& options);
    void next(const std::string& options);
    void previous(const std::string& options);
    void remove(const std::string& options);
    void help(const std::string& options);
    void list(const std::string& options);
    void quit(const std::string& options);

private:

    // TODO dont execute anything if db is empty
    // TODO maybe store as a reference
    std::map<unsigned, Notification> _db;

    std::map<unsigned, Notification>::iterator _iter;
};

} // namespace usbguardNotifier

#endif // NOTIFIER_CLI_HPP
