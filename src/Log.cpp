/*
 * Copyright (C) 2019 Red Hat, Inc.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Author(s): Attila Lakatos <alakatos@redhat.com>
 */

#include "Log.hpp"

#include <iostream>

namespace usbguardNotifier
{

Logger g_nLog;

Logger::Logger(bool debug)
    : _debug(debug) {}

LoggerStream Logger::createLogMessage(
    const std::string& file,
    const std::string& function,
    int line)
{
    return LoggerStream(*this, { file, function, line });
}

void Logger::setDebugMode(bool debug) noexcept
{
    _debug = debug;
}

bool Logger::isEnabled() const noexcept
{
    return _debug;
}

LoggerStream::LoggerStream(Logger& logger, Source source)
    : _logger(logger),
      _source(source) {}

LoggerStream::LoggerStream(const LoggerStream& rhs)
    : std::basic_ios<std::ostringstream::char_type, std::ostringstream::traits_type>(),
      std::ostringstream(rhs.str()),
      _logger(rhs._logger),
      _source(rhs._source) {}

LoggerStream::~LoggerStream()
{
    writeLogMessage();
}

void LoggerStream::writeLogMessage() const
{
    std::cout << "LOG: " <<  _source.file << "::";
    std::cout << _source.line;
    std::cout << " [" << _source.function << "] ";
    std::cout << str();
    std::cout << std::endl;
}

} // namespace usbuard-notifier
