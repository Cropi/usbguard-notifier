#ifndef NOTIFY_WRAPPER_HPP
#define NOTIFY_WRAPPER_HPP

#include <stdexcept>
#include <string>

#include <libnotify/notify.h>

namespace notify
{

/**
 * Notification API.
 */
class Notify
{
public:
    explicit Notify(const std::string& appName)
    {
        if (!notify_init(appName.c_str())) {
            throw std::runtime_error("Failed to initialize libnotify");
        }
    }
    Notify(const Notify&) = delete;
    Notify& operator=(const Notify&) = delete;
    ~Notify() { notify_uninit(); }
    std::string getAppName() const noexcept { return { notify_get_app_name() }; }
    void setAppName(const std::string& appName) { notify_set_app_name(appName.c_str()); }
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
     * @brief Creates a new Notification.
     * The summary text is required, but all other parameters are optional.
     *
     * @param summary The required summary text.
     * @param body The optional body text.
     * @param icon The optional icon, theme, icon name or filename.
     */
    Notification(const std::string& summary, const std::string& body = "", const std::string& icon = "")
        : _n(notify_notification_new(summary.c_str(), body.c_str(), icon.c_str())) {}

    Notification(const Notification&) = delete;
    Notification& operator=(const Notification&) = delete;
    ~Notification() = default;

    /**
     * @brief Updates the notification text and icon.
     * This won't send the update out and display it on the screen.
     * For that, you will need to call show().
     *
     * @param summary The new required summary text.
     * @param body The optional body text.
     * @param icon The optional icon, theme, icon name or filename.
     * @return True, unless an invalid parameter was passed.
     */
    bool update(const std::string& summary, const std::string& body = "", const std::string& icon = "")
    {
        return notify_notification_update(_n, summary.c_str(), body.c_str(), icon.c_str());
    }

    /**
     * @brief Tells the notification server to display the notification on the screen.
     *
     * @return True if successful, false otherwise.
     */
    bool show() const { return notify_notification_show(_n, nullptr); }

    /**
     * @brief Sets the timeout of the notification in milliseconds.
     * Note that the timeout may be ignored by the server.
     *
     * @param timeout Timeout in milliseconds.
     */
    void setTimeout(int timeout) noexcept { notify_notification_set_timeout(_n, timeout); }

    /**
     * @brief Sets the category of this notification.
     * This can be used by the notification server to filter or display the data in a certain way.
     *
     * @param category The category.
     */
    void setCategory(const std::string& category) noexcept { notify_notification_set_category(_n, category.c_str()); }

    /**
     * @brief Sets the urgency level of this notification.
     *
     * @param urgency The urgency level.
     * @see Urgency
     */
    void setUrgency(Urgency urgency) noexcept { notify_notification_set_urgency(_n, (NotifyUrgency)urgency); }

    /**
     * @brief Synchronously tells the notification server to hide the notification on the screen.
     *
     * @return True if successful, false otherwise.
     */
    bool close() const { return notify_notification_close(_n, nullptr); }

private:
    NotifyNotification* _n;
};

} /* namespace notify */
#endif /* NOTIFY_WRAPPER_HPP */
