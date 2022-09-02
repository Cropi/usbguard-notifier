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

#include "NotifyWrapper.hpp"
#include "usbguard-icon.hpp"

#include <cstring>
#include <stdexcept>

#include <librsvg-2.0/librsvg/rsvg.h>

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
    RsvgHandle* handle = rsvg_handle_new_from_data((const guint8*)icon, std::strlen(icon), nullptr);
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

void Notification::addAction(
    const std::string& action,
    const std::string& label,
    void (*callback)(NotifyNotification*, char*, gpointer),
    gpointer user_data) noexcept
{
    notify_notification_add_action(_n, action.c_str(), label.c_str(),
        (NotifyActionCallback)callback, user_data, NULL);
}

} // namespace notify
