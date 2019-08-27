#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include "NotifyWrapper.hpp"

#include <usbguard/IPCClient.hpp>

namespace usbguardNotifier
{

class Notifier : public usbguard::IPCClient
{
public:
    explicit Notifier(const std::string& appName)
        : _lib(appName) {}

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

    void PropertyParameterChanged(
        const std::string& name,
        const std::string& value_old,
        const std::string& value_new);

private:
    notify::Notify _lib;
};

} // namespace usbguardNotifier

#endif // NOTIFIER_HPP
