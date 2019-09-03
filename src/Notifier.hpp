#ifndef NOTIFIER_HPP
#define NOTIFIER_HPP

#include "NotifyWrapper.hpp"

#include <usbguard/IPCClient.hpp>
#include <map>

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

    void PropertyParameterChanged(
        const std::string& name,
        const std::string& value_old,
        const std::string& value_new);

private:
    std::map<std::string, std::string> parseConfigFile(std::string file_path);

    struct Data {
        const uint32_t id;
        const std::string event_type;
        const std::string device_name;
        const std::string target_old;
        const std::string target_new;
        const uint32_t rule_id;
        const std::string rule;
    };

    void storeNotification(const Data& data);

    notify::Notify _lib;
    std::map<std::string, std::string> _config;
};

} // namespace usbguardNotifier

#endif // NOTIFIER_HPP
