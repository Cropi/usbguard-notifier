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
 * Author(s): Zoltán Fridrich <zfridric@redhat.com>
 */

#ifndef NOTIFY_WRAPPER_HPP
#define NOTIFY_WRAPPER_HPP

#include <string>

#include <libnotify/notify.h>
#include <usbguard/Rule.hpp>

namespace usbguardNotifier
{
class Notifier;
}

namespace notify
{

/**
 * Notification API.
 */
class Notify
{
public:
    explicit Notify(const std::string& app_name);
    Notify(const Notify&) = delete;
    Notify& operator=(const Notify&) = delete;
    ~Notify();
    std::string getAppName() const noexcept;
    void setAppName(const std::string& app_name);
};

enum class Urgency {
    Low = NOTIFY_URGENCY_LOW,
    Normal = NOTIFY_URGENCY_NORMAL,
    Critical = NOTIFY_URGENCY_CRITICAL
};

/**
 * Notification represents a passive pop-up notification.
 * It can contain summary text, body text, and an icon, as well
 * as hints specifying how the notification should be presented.
 * The notification is rendered by a notification daemon and may
 * present the notification in any number of ways. As such, there
 * is a clear separation of content and presentation, and this API
 * enforces that.
 */
class Notification
{
public:
    /**
    * @brief a simple struct to pass multiple parameters in a Gtk callback.
    */
    struct AllowDeviceActionData {
        usbguard::Rule rule;
        usbguardNotifier::Notifier* notifier;
        AllowDeviceActionData(usbguard::Rule r, usbguardNotifier::Notifier* n) : rule(r), notifier(n) {}
    };


    /**
     * @brief Creates a new Notification with USBGuard icon.
     * The summary text is required, but all other parameters are optional.
     *
     * @param summary The required summary text.
     * @param body The optional body text.
     */
    Notification(const std::string& summary, const std::string& body = "");

    Notification(const Notification&) = delete;
    Notification& operator=(const Notification&) = delete;
    ~Notification() = default;

    /**
     * @brief Updates the notification text.
     * This won't send the update out and display it on the screen.
     * For that, you will need to call show().
     *
     * @param summary The new required summary text.
     * @param body The optional body text.
     * @return True, unless an invalid parameter was passed.
     */
    bool update(const std::string& summary, const std::string& body = "");

    /**
     * @brief Tells the notification server to display
     * the notification on the screen.
     *
     * @return True if successful, false otherwise.
     */
    bool show() const;

    /**
     * @brief Sets the timeout of the notification in milliseconds.
     * Note that the timeout may be ignored by the server.
     *
     * @param timeout Timeout in milliseconds.
     */
    void setTimeout(int timeout) noexcept;

    /**
    * @brief An utility method to replace all occurences of a
    *  substring in a string with another substring
    *
    * @param str the original string.
    * @param from the substring to search for.
    * @param to the string to replace with
    */
    static void replaceAll(std::string& str, const std::string& from,
        const std::string& to);


    /**
    * @brief Sets the action button to allow the device.
    * pressing should open a prompt asking for the admin password
    * before creating a new rule on USBGuard's rules.conf
    *
    * @param rule The rule to be added.
    * @param notifier The notifier object to send the IPC message.
    */
    void setAllowAction(usbguard::Rule& rule,
        usbguardNotifier::Notifier* notifier) noexcept;


    /**
    * @brief allow device by adding a new USBGuard rule.
    * as in this PoC 2 methods of doing so are presented
    * (IPC and system command), this method contains code
    * shared by both solutions, and calls one. (the other is commented)
    * called when "allow" action is pressed.

    * @param notification notification object, to dismiss it after click.
    * @param action unused.
    * @param user_data struct with the rule to add and IPC client.
    */
    static void allowDeviceActionCallback(NotifyNotification* notification,
        char* action, gpointer user_data);


    /**
    * @brief adds new allow rule with the usbguard command from a subshell.
    * uses pkexec to elevate privileges.
    *
    * @param rule the rule to add to configuration.
    * @param isPermanent keep rule for the session or forever.
    */
    static void allowDeviceSystemCommand(usbguard::Rule& rule, bool isPermanent);

    /**
    * @brief adds new allow rule with the usbguard IPC client.
    * client must have the rights to modify USBGuard policy
    *
    * @param AllowDeviceActionData struct object with the rule and IPC client.
    * @param isPermanent keep rule for the session or forever.
    */
    static void allowDeviceNotifierIPC(AllowDeviceActionData* data,
        bool isPermanent);

    /**
     * @brief Sets the category of this notification.
     * This can be used by the notification server to filter
     * or display the data in a certain way.
     *
     * @param category The category.
     */
    void setCategory(const std::string& category) noexcept;

    /**
     * @brief Sets the urgency level of this notification.
     *
     * @param urgency The urgency level.
     * @see Urgency
     */
    void setUrgency(Urgency urgency) noexcept;

    /**
     * @brief Synchronously tells the notification server
     * to hide the notification on the screen.
     *
     * @return True if successful, false otherwise.
     */
    bool close() const;

private:
    NotifyNotification* _n;
};

} // namespace notify

#endif // NOTIFY_WRAPPER_HPP
