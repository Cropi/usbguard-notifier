#include "NotifyWrapper.hpp"

#include <stdexcept>

#include <librsvg-2.0/librsvg/rsvg.h>

extern char _binary_icons_usbguard_icon_svg_start[];
extern char _binary_icons_usbguard_icon_svg_end[];

namespace notify
{

Notify::Notify(const std::string& app_name)
{
    if (!notify_init(app_name.c_str())) {
        throw std::runtime_error("Failed to initialize libnotify");
    }
}

Notify::~Notify()
{
    notify_uninit();
}

std::string Notify::getAppName() const noexcept
{
    return { notify_get_app_name() };
}

void Notify::setAppName(const std::string& app_name)
{
    notify_set_app_name(app_name.c_str());
}

Notification::Notification(const std::string& summary, const std::string& body)
    : _n(notify_notification_new(summary.c_str(), body.c_str(), nullptr))
{
    RsvgHandle* handle = rsvg_handle_new_from_data(
            (const guint8*)(_binary_icons_usbguard_icon_svg_start),
            _binary_icons_usbguard_icon_svg_end - _binary_icons_usbguard_icon_svg_start,
            nullptr);
    if (!handle) {
        throw std::runtime_error("Failed to obtain rsvg handle");
    }
    GdkPixbuf* pixbuf = rsvg_handle_get_pixbuf(handle);
    notify_notification_set_image_from_pixbuf(_n, pixbuf);
    g_object_unref(handle);
}

bool Notification::update(const std::string& summary, const std::string& body)
{
    return notify_notification_update(_n, summary.c_str(), body.c_str(), nullptr);
}

bool Notification::show() const
{
    return notify_notification_show(_n, nullptr);
}

void Notification::setTimeout(int timeout) noexcept
{
    notify_notification_set_timeout(_n, timeout);
}

void Notification::setCategory(const std::string& category) noexcept
{
    notify_notification_set_category(_n, category.c_str());
}

void Notification::setUrgency(Urgency urgency) noexcept
{
    notify_notification_set_urgency(_n, (NotifyUrgency)urgency);
}

bool Notification::close() const
{
    return notify_notification_close(_n, nullptr);
}

} // namespace notify
