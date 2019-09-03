#include "Log.hpp"
#include "Notifier.hpp"
#include "NotifyWrapper.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

#include <usbguard/DeviceManager.hpp>
#include <usbguard/Rule.hpp>

#include "build-config.h"

namespace usbguardNotifier
{

Notifier::Notifier(const std::string& app_name) :
    _lib(app_name)
{
    _config = parseConfigFile(CONF_FILE);
}

void Notifier::DevicePolicyChanged(
    uint32_t id,
    usbguard::Rule::Target target_old, usbguard::Rule::Target target_new,
    const std::string& device_rule,
    uint32_t rule_id)
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

    const Notifier::Data data = {
        id, __func__, rule.getName(), targetOldStr, targetNewStr, rule_id, device_rule
    };
    storeNotification(data);
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

void Notifier::storeNotification(const Notifier::Data& data)
{
    NOTIFIER_LOG() << "Store notification";

    std::ofstream f;
    f.open(_config["NotificationPath"], std::ios::app | std::ios::out);
    if (!f.good()) {
        std::cerr << "Could not open " << _config["NotificationPath"] << std::endl;
        return;
    }

    std::string message;
    std::time_t time = std::time(nullptr);
    message.append(std::asctime(std::localtime(&time)));
    message.pop_back(); // delete the new line character, appended by std::asctime function
    message.append(" ");

    message.append(data.event_type);
    message.append(" on=");
    message.append(data.device_name);
    message.append(" from=");
    message.append(data.target_old);
    message.append(" to=");
    message.append(data.target_new);
    message.append(" with-rule=");
    message.append(data.rule);

    f << message << std::endl;

    f.close();
}

} // namespace usbguardNotifier
