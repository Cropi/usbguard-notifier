#include "Serializer.hpp"

#include <stdexcept>

namespace usbguardNotifier
{

static char delim = ';';

bool operator==(const Notification& lhs, const Notification& rhs)
{
    return lhs.event_type == rhs.event_type
        && lhs.device_id == rhs.device_id
        && lhs.device_name == rhs.device_name
        && lhs.target_old == rhs.target_old
        && lhs.target_new == rhs.target_new
        && lhs.rule_id == rhs.rule_id
        && lhs.rule == rhs.rule;
}

Serializer::Serializer(std::string file_name)
    : _file_name(std::move(file_name)) {}

const std::string& Serializer::getFileName() const noexcept
{
    return _file_name;
}

void Serializer::setFileName(std::string file_name) noexcept
{
    _file_name = std::move(file_name);
}

bool Serializer::serialize(const Notification& n) const
{
    std::ofstream out(_file_name, std::ios_base::app);
    if (!out) {
        return false;
    }
    out << n.event_type << delim << n.device_id << delim << n.device_name
        << delim << n.target_old << delim << n.target_new << delim
        << n.rule_id << delim << n.rule << std::endl;
    return (bool)out;
}

Notification Serializer::deserialize() const
{
    std::ifstream in(_file_name);
    if (!in) {
        throw std::runtime_error("Couldnt open file for deserialization.");
    }
    Notification n;
    if (!deserialize(in, n)) {
        throw std::runtime_error("Deserialization failed.");
    }
    return n;
}

std::map<unsigned, Notification> Serializer::deserializeAll() const
{
    std::ifstream in(_file_name);
    if (!in) {
        throw std::runtime_error("Couldnt open file for deserialization.");
    }
    std::map<unsigned, Notification> v;
    Notification n;
    for (unsigned i = 1; deserialize(in, n); ++i) {
        v.insert({ i, n });
    }
    return v;
}

bool Serializer::deserialize(std::ifstream& in, Notification& n) const
{
    std::getline(in, n.event_type, delim);
    in >> n.device_id;
    in.get();
    std::getline(in, n.device_name, delim);
    std::getline(in, n.target_old, delim);
    std::getline(in, n.target_new, delim);
    in >> n.rule_id;
    in.get();
    std::getline(in, n.rule);
    return (bool)in;
}

} // namespace usbguardNotifier
