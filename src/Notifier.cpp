#include "Notifier.hpp"
#include "Log.hpp"

#include <string>

namespace usbguardNotifier
{
    void SignalWatcher::DevicePolicyChanged(
        uint32_t id,
        usbguard::Rule::Target target_old, usbguard::Rule::Target target_new,
        const std::string& device_rule,
        uint32_t rule_id)
    {
        NOTIFIER_LOG() << "Device policy changed signal";
        // TODO
    }

    void SignalWatcher::DevicePresenceChanged(
        uint32_t id,
        usbguard::DeviceManager::EventType event,
        usbguard::Rule::Target target,
        const std::string& device_rule)
    {
        NOTIFIER_LOG() << "Device presence changed signal";
        // TODO
    }

    void SignalWatcher::PropertyParameterChanged(
        const std::string& name,
        const std::string& value_old,
        const std::string& value_new)
    {
        NOTIFIER_LOG() << "Property parameter changed signal";
        // TODO
    }

} /* namespace usbguardNotifier */






















/**/
