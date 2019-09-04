#ifndef LOG_HPP
#define LOG_HPP

#include <string>
#include <sstream>

#define NOTIFIER_LOGGER usbguardNotifier::g_nLog

#define NOTIFIER_LOG() \
    if (NOTIFIER_LOGGER.isEnabled()) \
        usbguardNotifier::g_nLog.createLogMessage(__BASE_FILE__, __func__, __LINE__)

namespace usbguardNotifier
{

class LoggerStream;

class Logger
{
public:
    Logger(bool debug = false);

    LoggerStream createLogMessage(
        const std::string& file,
        const std::string& function,
        int line);

    void setDebugMode(bool debug) noexcept;
    bool isEnabled() const noexcept;

private:
    bool _debug;
};

class LoggerStream : public std::ostringstream
{
public:
    struct Source {
        const std::string& file;
        const std::string& function;
        const int line;
    };

    LoggerStream(Logger& logger, Source source);
    LoggerStream(const LoggerStream& rhs);
    ~LoggerStream();

private:
    void writeLogMessage() const;

    Logger _logger;
    Source _source;
};

extern Logger g_nLog;

} // namespace usbguardNotifier

#endif
