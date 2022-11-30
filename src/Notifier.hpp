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

    /**
     * @brief Notification action's C-style callback
     * This callback later calls private method 'actionsCallback' which is the "desired"
     * callback as it can access class members.
     *
     * @see \link notify::Notification::addAction addAction\endlink
     *
     * @param n Not in use. Imposed by libnotify API.
     * @param action Action ID.
     * @param user_data Pointer to _actionsCallbackUserData in an object instance.
     */
    static void actionsCallbackWrapper(
        NotifyNotification* n,
        char* action,
        gpointer user_data);

private:
    struct DevicePresenceInfo {
        bool isInitialized = false;
        uint32_t id;
        usbguard::DeviceManager::EventType event;
        usbguard::Rule::Target target;
        std::string device_rule;

        DevicePresenceInfo(const uint32_t id,
            const usbguard::DeviceManager::EventType& e,
            const usbguard::Rule::Target& t,
            const std::string& r): isInitialized(true), id(id), event(e), target(t), device_rule(r) {}

        DevicePresenceInfo() : id(0), event(), target(), device_rule() {}
    };

    /**
     * @brief Structure passed as argument in notification action's callback.
     *
     * @see \link Notification::addAction addAction\endlink
     * @see \link Notifier::actionsCallback actionsCallback\endlink
     */
    struct ActionsCallbackUserData {
        Notifier* object_ptr; /**< Reference to class instance. */
        DevicePresenceInfo* info; /**< Device information needed by \link actionsCallback actionsCallback\endlink method. */

        ActionsCallbackUserData(Notifier* ctx, DevicePresenceInfo* info) : object_ptr(ctx), info(info) {};

        ActionsCallbackUserData() : object_ptr(nullptr), info(nullptr) {};
    };

    DevicePresenceInfo getDevicePresenceObject(uint32_t id);

    void sendDevicePresenceCountdownCallback(uint32_t id);

    void sendDevicePresenceNotification(DevicePresenceInfo& info);

    void actionsCallback(const std::string& action_id, DevicePresenceInfo* info);

    notify::Notify _lib;
    Serializer _ser;
    std::map<uint32_t, DevicePresenceInfo> _deviceNotifications;
    std::mutex _mtx;
    std::vector<std::thread*> _countdownThreads;
    const int _kMillisecondsDevicePolicyWait = 500;

    GMainLoop* _GMLoop;
    std::thread* _GMLoopThread;

    ActionsCallbackUserData _actionsCallbackUserData;
};

} // namespace usbguardNotifier

#endif // NOTIFIER_HPP
