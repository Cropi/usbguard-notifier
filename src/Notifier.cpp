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
 * Author(s): Attila Lakatos <alakatos@redhat.com>
 */

#include "BuildConfig.h"
#include "Common.hpp"
#include "Log.hpp"
#include "Notifier.hpp"

#include <sstream>
#include <stdexcept>
#include <string>

#include <usbguard/DeviceManager.hpp>
#include <usbguard/Rule.hpp>
#include <usbguard/Exception.hpp>

namespace usbguardNotifier
{

Notifier::Notifier(const std::string& app_name) :
    _lib(app_name),
    _cfg(g_nconfig_names)
{
    try {
        _cfg.open(CONF_FILE, /*readonly=*/true);
        _ser.setFileName(_cfg.getSettingValue("NotificationPath"));
    } catch (const usbguard::Exception& e) {
        throw std::runtime_error("Unable to open configuration file");
    }
}

void Notifier::DevicePolicyChanged(
    uint32_t id,
    usbguard::Rule::Target target_old,
    usbguard::Rule::Target target_new,
    const std::string& device_rule,
    uint32_t rule_id)
{
    using namespace usbguard;
    NOTIFIER_LOG() << "Device policy changed signal";

    const std::string target_old_str = Rule::targetToString(target_old);
    const std::string target_new_str = Rule::targetToString(target_new);
    Rule rule = Rule::fromString(device_rule);

    std::ostringstream body;
    body << rule.getName() << ": " << target_new_str;

    notify::Notification n("USBGuard", body.str());
    if (!n.show()) {
        throw std::runtime_error("Failed to show notification");
    }
    NOTIFIER_LOG() << "Store notification";
    Notification obj = { __func__, id, rule.getName(), target_old_str,
                     target_new_str, rule_id, device_rule
                 };
    _ser.serialize(obj);
}

void Notifier::DevicePresenceChanged(
    uint32_t /*id*/,
    usbguard::DeviceManager::EventType event,
    usbguard::Rule::Target target,
    const std::string& device_rule)
{
    using namespace usbguard;
    NOTIFIER_LOG() << "Device presence changed signal";

    const std::string event_str = DeviceManager::eventTypeToString(event);
    const std::string target_str = Rule::targetToString(target);
    Rule rule = Rule::fromString(device_rule);

    std::ostringstream body;
    body << event_str << ' ' << rule.getName() << ": " << target_str;

    notify::Notification n("USBGuard", body.str());
    n.setTimeout(5000);
    n.setCategory("device");
    if (!n.show()) {
        throw std::runtime_error("Failed to show notification");
    }
    // TODO serialize
}

} // namespace usbguardNotifier
