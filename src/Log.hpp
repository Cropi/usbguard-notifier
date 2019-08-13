#include <string>
#include <sstream>
#include <fstream>

namespace usbguardNotifier
{
class LoggerStream;
class Logger
{
public:
    Logger();
    LoggerStream createLogMessage(const std::string& file, const std::string& function, int line);
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
    void writeLogMessage();

    Logger _logger;
    Source _source;
};

extern Logger g_nLog;

#define NOTIFIER_LOG() \
    usbguardNotifier::g_nLog.createLogMessage(__BASE_FILE__, __func__, __LINE__)

} /* namespace usbguard-notifier */
