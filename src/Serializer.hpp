#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <fstream>
#include <string>
#include <vector>

namespace usbguardNotifier {

/**
 * Structure representing usbguard notification.
 */
struct Notification {
    int32_t deviceId;
    std::string deviceName;
    std::string targetOld;
    std::string targetNew;
    uint32_t ruleId;
    std::string rule;
};

/**
 * Class used to serialize notifications for usbguard-notifier.
 */
class Serializer {
public:
    /**
     * @brief Constructs serializer.
     *
     * @param fileName Name of the file on which Serializer operates.
     */
    explicit Serializer(std::string fileName)
    : _fileName(std::move(fileName)) {}

    /**
     * @brief Returns the name of the file on which Serializer operates.
     *
     * @return Name of the file on which Serializer operates.
     */
    const std::string& getFileName() const noexcept { return _fileName; }

    /**
     * @brief Sets the name of the file on which Serializer operates.
     *
     * @param Name of the file on which Serializer operates.
     */
    void setFile(std::string fileName) noexcept { _fileName = std::move(fileName); }
   
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
    std::vector<Notification> deserializeAll() const;
    
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
    std::string _fileName;
};

} // namespace usbguardNotifier

#endif // SERIALIZER_HPP
