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
#include "Log.hpp"
#include "Notifier.hpp"

#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <chrono>
#include <iostream>

#include <usbguard/DeviceManager.hpp>
#include <usbguard/Rule.hpp>
#include <usbguard/Exception.hpp>

namespace usbguardNotifier
{

Notifier::Notifier(const std::string& app_name) :
    _lib(app_name)
{
    std::string login = getlogin() ? getlogin() : "unknown";
    std::string path(std::string(NOTIFICATION_DIR) + "/" + login);
    _ser.setFileName(path);

    // g_mail_loop is required by action's callback
    _GMLoop = g_main_loop_new(nullptr, FALSE);
    _GMLoopThread = new std::thread( [this] { g_main_loop_run(_GMLoop); } );

    _actionsCallbackUserData.object_ptr = this;
    _actionsCallbackUserData.info = nullptr;
}

Notifier::~Notifier()
{
    for (auto t : _countdownThreads) {
        t->join();
        delete t;
    }

    g_main_loop_quit(_GMLoop);
    _GMLoopThread->join();
    delete _GMLoopThread;
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

    DevicePresenceInfo info = getDevicePresenceObject(id);
    if (info.isInitialized) {
        if (info.target == target_old ||
            device_rule.substr(target_new_str.size()) == info.device_rule.substr(target_old_str.size())) {
            info.target = target_new;
            sendDevicePresenceNotification(info);
            return;
        } else {
            NOTIFIER_LOG() << "DevicePolicyChanged and DevicePresenceChanged " <<
                "with same id do not share the same rule.";
        }
    }

    Rule rule = Rule::fromString(device_rule);

    std::ostringstream body;
    body << rule.getName() << ": " << target_new_str << "ed";

    notify::Notification n("Update - USBGuard", body.str());
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
    uint32_t id,
    usbguard::DeviceManager::EventType event,
    usbguard::Rule::Target target,
    const std::string& device_rule)
{
    using namespace usbguard;
    NOTIFIER_LOG() << "Device presence changed signal";

    _deviceNotifications.emplace(std::make_pair(id, DevicePresenceInfo(id, event, target, device_rule)));
    std::thread* t = new std::thread( [this, id] { sendDevicePresenceCountdownCallback(id); } );
    _countdownThreads.push_back(t);
}

Notifier::DevicePresenceInfo Notifier::getDevicePresenceObject(uint32_t id)
{
    DevicePresenceInfo info;
    _mtx.lock();
    auto it = _deviceNotifications.find(id);
    if (it != _deviceNotifications.end()) {
        info = it->second;
        _deviceNotifications.erase(it);
    }
    _mtx.unlock();
    return info;
}

void Notifier::sendDevicePresenceCountdownCallback(uint32_t id)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(_kMillisecondsDevicePolicyWait));

    DevicePresenceInfo info = getDevicePresenceObject(id);
    if (info.isInitialized) {
        sendDevicePresenceNotification(info);
    }
}

void Notifier::sendDevicePresenceNotification(DevicePresenceInfo& info)
{
    using namespace usbguard;

    const std::string event_str = DeviceManager::eventTypeToString(info.event);
    const std::string target_str = Rule::targetToString(info.target);
    Rule rule = Rule::fromString(info.device_rule);

    std::ostringstream body;
    if (info.event == DeviceManager::EventType::Remove) {
        body << rule.getName();
    } else {
        body << rule.getName() << ": " << target_str << "ed";
    }

    notify::Notification n(event_str + " - USBGuard", body.str());
    n.setTimeout(5000);
    n.setCategory("device");

    if (info.event == DeviceManager::EventType::Insert) {
        _actionsCallbackUserData.info = &info;
        if (info.target != Rule::Target::Allow) {
            n.addAction(Rule::targetToString(Rule::Target::Allow), "Allow",
                &Notifier::actionsCallbackWrapper,
                &_actionsCallbackUserData);
        }
        n.addAction(Rule::targetToString(Rule::Target::Reject), "Reject",
            &Notifier::actionsCallbackWrapper,
            &_actionsCallbackUserData);
    }

    if (!n.show()) {
        throw std::runtime_error("Failed to show notification");
    }
    // TODO serialize
}

void Notifier::actionsCallback(const std::string& action_id, DevicePresenceInfo* info)
{
    using namespace usbguard;

    try {
        if (Rule::targetFromString(action_id)  == Rule::Target::Allow) {
            IPCClient::applyDevicePolicy(info->id, usbguard::Rule::Target::Allow, false);
        } else if (Rule::targetFromString(action_id)  == Rule::Target::Reject) {
            IPCClient::applyDevicePolicy(info->id, usbguard::Rule::Target::Reject, false);
        }
        NOTIFIER_LOG() << "Device Policy '" << action_id
            << "' applied to device with ID: " << info->id;
    } catch (const usbguard::IPCException& ex) {
        std::cerr << "IPC call failed while trying to apply device policy"
            << " '" << action_id << "' over device with ID " << info->id
            << " with message:" << std::endl;
        std::cerr << ex.message() << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "IPC call failed while trying to apply device policy"
            << " '" << action_id << "' over device with ID " << info->id
            << " with message:" << std::endl;
        std::cerr << ex.what() << std::endl;
    }
}

void Notifier::actionsCallbackWrapper(NotifyNotification* n __attribute__((unused)), char* action, gpointer user_data)
{
    using namespace usbguardNotifier;

    // Cast struct here for better readability
    auto ud = static_cast<Notifier::ActionsCallbackUserData*>(user_data);

    // Call member callback
    ud->object_ptr->actionsCallback(action, ud->info);
}

} // namespace usbguardNotifier
