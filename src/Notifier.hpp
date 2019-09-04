#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include "build-config.h"
#include "NotifyWrapper.hpp"
#include "Serializer.hpp"

#include <map>

#include <usbguard/IPCClient.hpp>
#include <usbguard/ConfigFile.hpp>

namespace usbguardNotifier
{

class Notifier : public usbguard::IPCClient
{
public:
    Notifier(const std::string& app_name);

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
    notify::Notify _lib;
    usbguard::ConfigFile _config;
    Serializer _s;
};

} // namespace usbguardNotifier

#endif // NOTIFIER_HPP
