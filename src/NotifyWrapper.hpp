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
#include <memory>

#include <libnotify/notify.h>

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
     * @brief Creates a new Notification with USBGuard icon.
     * The summary text is required, but all other parameters are optional.
     *
     * @param summary The required summary text.
     * @param body The optional body text.
     */
    Notification(const std::string& summary, const std::string& body = "");

    Notification(const Notification&) = delete;
    Notification& operator=(const Notification&) = delete;
    ~Notification();

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
     * @brief Adds an action to the notification.
     *
     * libnotify uses a C-style callback so this type is needed as an argument.
     * A static function is needed for this callback to work.
     *
     * @param action The action ID.
     * @param label Human-readable action label. Shown in the notification.
     * @param callback C-style callback as defined by libnotify.
     * Callback parameter 'n' should not be used; 'action' and 'user_data' are the same.
     * @param user_data Optional custom data to pass to callback.
     */
    void addAction(
        const std::string& action,
        const std::string& label,
        void (*callback)(NotifyNotification* n, char* action, gpointer user_data),
        gpointer user_data) noexcept;

    /**
     * @brief Synchronously tells the notification server
     * to hide the notification on the screen.
     *
     * @return True if successful, false otherwise.
     */
    bool close() const;

private:
    struct NotificationDeleter {
        void operator()(NotifyNotification* n) const noexcept
        {
            if (n) {
                g_object_unref(G_OBJECT(n));
            }
        }
    };
    std::unique_ptr<NotifyNotification, NotificationDeleter> _n;
};

} // namespace notify

#endif // NOTIFY_WRAPPER_HPP
