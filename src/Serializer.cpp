#include "Serializer.hpp"

#include <stdexcept>

namespace usbguardNotifier {

static char delim = ';';

bool Serializer::serialize(const Notification& n) const {
    std::ofstream out(_fileName, std::ios_base::app);
    if (!out) return false;
    out << n.eventType << delim << n.deviceId << delim << n.deviceName
        << delim << n.targetOld << delim << n.targetNew << delim
        << n.ruleId << delim << n.rule << std::endl;
    return (bool)out;
}

Notification Serializer::deserialize() const {
    std::ifstream in(_fileName);
    if (!in) throw std::runtime_error("Couldnt open file for deserialization.");
    Notification n;
    if (!deserialize(in, n)) throw std::runtime_error("Deserialization failed.");
    return n;
}

std::vector<Notification> Serializer::deserializeAll() const {
    std::ifstream in(_fileName);
    if (!in) throw std::runtime_error("Couldnt open file for deserialization.");
    std::vector<Notification> v;
    Notification n;
    while (deserialize(in, n)) v.push_back(n);
    return v;
}

bool Serializer::deserialize(std::ifstream& in, Notification& n) const {
    std::getline(in, n.eventType, delim);
    in >> n.deviceId;
    in.get();
    std::getline(in, n.deviceName, delim);
    std::getline(in, n.targetOld, delim);
    std::getline(in, n.targetNew, delim);
    in >> n.ruleId;
    in.get();
    std::getline(in, n.rule);
    return (bool)in;
}

} // namespace usbguardNotifier
