#include "IPCMessage.hpp"
#include <map>


namespace usbguardNotifier
{
    namespace IPCMessage
    {
        static std::map<uint32_t, std::string> notifier_events {
            // TODO
            { 0x03, "usbguardNotifier.IPCMessage.deviceAvailabilityChanged" },
            { 0x04, "usbguardNotifier.IPCMessage.devicePolicyChanged" }
        };

        uint32_t payloadTypeNameToNumber(std::string payload_type)
        {
            // TODO
            return 1;
        }

    }
} /* namespace usbguardNotifier */
