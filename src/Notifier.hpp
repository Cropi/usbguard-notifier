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

#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include "NotifyWrapper.hpp"
#include "Serializer.hpp"

#include <usbguard/ConfigFile.hpp>
#include <usbguard/IPCClient.hpp>

#include <thread>
#include <mutex>

namespace usbguardNotifier
{

class Notifier : public usbguard::IPCClient
{
public:
    explicit Notifier(const std::string& app_name);
    ~Notifier();

    void DevicePolicyChanged(
        uint32_t id,
        usbguard::Rule::Target target_old,
        usbguard::Rule::Target target_new,
        const std::string& device_rule,
        uint32_t rule_id) override;

    void DevicePresenceChanged(
        uint32_t id,
        usbguard::DeviceManager::EventType event,
        usbguard::Rule::Target target,
        const std::string& device_rule) override;
    /*
        void PropertyParameterChanged(
            const std::string& name,
            const std::string& value_old,
            const std::string& value_new);
    */
private:
    struct DevicePresenceInfo {
        bool isInitialized = false;
        usbguard::DeviceManager::EventType event;
        usbguard::Rule::Target target;
        std::string device_rule;

        DevicePresenceInfo(const usbguard::DeviceManager::EventType& e,
            const usbguard::Rule::Target& t,
            const std::string& r): isInitialized(true), event(e), target(t), device_rule(r) {}

        DevicePresenceInfo() : event(), target(), device_rule() {}
    };

    DevicePresenceInfo getDevicePresenceObject(uint32_t id);

    void sendDevicePresenceCountdownCallback(uint32_t id);

    void sendDevicePresenceNotification(DevicePresenceInfo& info);

    notify::Notify _lib;
    Serializer _ser;
    std::map<uint32_t, DevicePresenceInfo> deviceNotifications;
    std::mutex mtx;
    std::vector<std::thread*> countdownThreads;
    int millisecondsDevicePolicyWait = 500;
};

} // namespace usbguardNotifier

#endif // NOTIFIER_HPP
