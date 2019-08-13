#include <iostream>

#include <usbguard/IPCClient.hpp>

namespace usbguardNotifier
{
    class SignalWatcher : public usbguard::IPCClient
    {
    public:

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
    };

} /* namespace usbguardNotifier */
