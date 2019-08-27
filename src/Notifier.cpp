#include "Log.hpp"
#include "Notifier.hpp"
#include "NotifyWrapper.hpp"

#include <iostream>
#include <string>

#include <usbguard/DeviceManager.hpp>
#include <usbguard/Rule.hpp>

namespace usbguardNotifier
{

void Notifier::DevicePolicyChanged(
    uint32_t /*id*/,
    usbguard::Rule::Target target_old, usbguard::Rule::Target target_new,
    const std::string& device_rule,
    uint32_t /*rule_id*/)
{
    NOTIFIER_LOG() << "Device policy changed signal";
    using namespace usbguard;

    const std::string targetOldStr = Rule::targetToString(target_old);
    const std::string targetNewStr = Rule::targetToString(target_new);
    Rule rule = Rule::fromString(device_rule);

    std::ostringstream body;
    body << rule.getName() << ": " << targetNewStr;

    notify::Notification n("USBGuard", body.str());
    if (!n.show()) {
        // TODO throw exception
    }
}

void Notifier::DevicePresenceChanged(
    uint32_t /*id*/,
    usbguard::DeviceManager::EventType event,
    usbguard::Rule::Target target,
    const std::string& device_rule)
{
    NOTIFIER_LOG() << "Device presence changed signal";
    using namespace usbguard;

    const std::string eventStr = usbguard::DeviceManager::eventTypeToString(event);
    const std::string targetStr = usbguard::Rule::targetToString(target);
    Rule rule = Rule::fromString(device_rule);

    std::ostringstream body;
    body << eventStr << ' ' << rule.getName() << ": " << targetStr;

    notify::Notification n("USBGuard", body.str());
    n.setTimeout(5000);
    n.setCategory("device");
    if (!n.show()) {
        // TODO throw exception
    }
}

void Notifier::PropertyParameterChanged(
    const std::string& /*name*/,
    const std::string& /*value_old*/,
    const std::string& /*value_new*/)
{
    NOTIFIER_LOG() << "Property parameter changed signal";
    // TODO
}

} // namespace usbguardNotifier
