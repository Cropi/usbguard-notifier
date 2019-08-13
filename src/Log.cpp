#include "src/Log.hpp"

#include <iostream>

namespace usbguardNotifier
{
Logger g_nLog;

LoggerStream::LoggerStream(Logger& logger, Source source)
    : _logger(logger),
      _source(source)
{
}

LoggerStream::LoggerStream(const LoggerStream& rhs)
    : std::basic_ios<std::ostringstream::char_type, std::ostringstream::traits_type>(),
      std::ostringstream(rhs.str()),
      _logger(rhs._logger),
      _source(rhs._source)
{
}

LoggerStream::~LoggerStream()
{
    writeLogMessage();
}

void LoggerStream::writeLogMessage()
{
    std::cout << "LOG: " <<  _source.file << "::";
    std::cout << _source.line;
    std::cout << " [" << _source.function << "] ";
    std::cout << str();
    std::cout << std::endl;
}

Logger::Logger() {}

LoggerStream Logger::createLogMessage(const std::string& file, const std::string& function, int line)
{
    const LoggerStream::Source source = {
        file,
        function,
        line
    };
    return LoggerStream(*this, source);
}
} /* namespace usbuard-notifier */
