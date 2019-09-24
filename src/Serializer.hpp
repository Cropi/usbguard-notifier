#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <fstream>
#include <map>
#include <string>

namespace usbguardNotifier
{

/**
 * Structure representing usbguard notification.
 */
struct Notification {
    std::string event_type;
    uint32_t device_id;
    std::string device_name;
    std::string target_old;
    std::string target_new;
    uint32_t rule_id;
    std::string rule;
};

bool operator==(const Notification& lhs, const Notification& rhs);

/**
 * Class used to serialize notifications for usbguard-notifier.
 */
class Serializer
{
public:
    /**
     * @brief Constructs serializer without initializing _file_name
     */
    Serializer() = default;

    /**
     * @brief Constructs serializer.
     *
     * @param file_name Name of the file on which Serializer operates.
     */
    explicit Serializer(std::string file_name);

    /**
     * @brief Returns the name of the file on which Serializer operates.
     *
     * @return Name of the file on which Serializer operates.
     */
    const std::string& getFileName() const noexcept;

    /**
     * @brief Sets the name of the file on which Serializer operates.
     *
     * @param file_name Name of the file on which Serializer operates.
     */
    void setFileName(std::string file_name) noexcept;

    /**
     * @brief Serializes given notification into the file.
     *
     * @param n Notification to serialize.
     * @return True if serialization succeeded, false otherwise.
     */
    bool serialize(const Notification& n) const;

    /**
     * @brief Deserializes and returns a single notification.
     *
     * @return Deserialized notification from the beggining of the file.
     */
    Notification deserialize() const;

    /**
     * @brief Deserializes and returns all notifications in a vector.
     *
     * @return All notifications from the file.
     */
    std::map<unsigned, Notification> deserializeAll() const;

private:

    /**
     * @brief Core deserialization function.
     *
     * @param in Stream from which notification will be deserialized.
     * @param n Notification to fill.
     * @return True if deserialization succeeded, false otherwise.
     */
    bool deserialize(std::ifstream& in, Notification& n) const;

    /**
     * Name of the file on which Serializer operates.
     */
    std::string _file_name;
};

} // namespace usbguardNotifier

#endif // SERIALIZER_HPP
