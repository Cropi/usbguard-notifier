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
