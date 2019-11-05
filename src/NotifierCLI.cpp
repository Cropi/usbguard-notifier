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
 *            Zoltán Fridrich <zfridric@redhat.com>
 */

#include "NotifierCLI.hpp"

#include <iostream>
#include <iterator>

namespace usbguardNotifier
{

struct cmd_cmp {
    bool operator()(const std::string& lhs, const std::string& rhs) const
    {
        return (lhs.length() == 1 || rhs.length() == 1)
            ? lhs.front() < rhs.front()
            : lhs < rhs;
    }
};

struct cmd_data {
    CLI::Command code;
    void(CLI::*command)(const std::string&);
    std::string description;
};

static const std::map<std::string, cmd_data, cmd_cmp> commands = {
    {
        "show", {
            CLI::Command::SHOW, &CLI::show,
            "Show all notifications."
        }
    },
    {
        "display", {
            CLI::Command::DISPLAY, &CLI::display,
            "Display detailed description of current notification."
        }
    },
    {
        "jump", {
            CLI::Command::JUMP, &CLI::jump,
            "Jump to the specified index."
        }
    },
    {
        "next", {
            CLI::Command::NEXT, &CLI::next,
            "Move cursor to next notification."
        }
    },
    {
        "previous", {
            CLI::Command::PREVIOUS, &CLI::previous,
            "Move cursor to previous notification."
        }
    },
    {
        "help", {
            CLI::Command::HELP, &CLI::help,
            "Show this help."
        }
    },
    {
        "list", {
            CLI::Command::LIST, &CLI::list,
            "List all available commands."
        }
    },
    {
        "quit", {
            CLI::Command::QUIT, &CLI::quit,
            "Quit CLI."
        }
    }
};

CLI::CLI(const CLI::map& db)
    : _db(db),
      _iter(_db.cbegin()) {}

CLI::Command CLI::execute(
    const std::string& cmd_name,
    const std::string& cmd_options)
{
    const auto iterator = commands.find(cmd_name);
    if (iterator == commands.cend()) {
        return CLI::Command::UNKNOWN;
    }
    auto command = iterator->second.command;
    (*this.*command)(cmd_options);
    return iterator->second.code;
}

void CLI::show(const std::string& options)
{
    for (const auto& x : _db) {
        std::string line = std::string(x.first == _iter->first ? ">" : " ")
            + "#" + std::to_string(x.first) + ": " + x.second.event_type
            + " <" + x.second.device_name + "> " + x.second.target_new;
        if (line.find(options) != std::string::npos) {
            std::cout << line << std::endl;
        }
    }
}

void CLI::display(const std::string& /*options*/)
{
    std::cout << "Notification #" << _iter->first << ":\n";
    std::cout << "    Name:      " << _iter->second.device_name << '\n';
    std::cout << "    Type:      " << _iter->second.event_type << '\n';
    std::cout << "    Old value: " << _iter->second.target_old << '\n';
    std::cout << "    New value: " << _iter->second.target_new << '\n';
    std::cout << "    Rule:      " << _iter->second.rule << std::endl;
}

void CLI::jump(const std::string& options)
{
    try {
        auto i = _db.find(static_cast<unsigned>(std::stoul(options)));
        if (i == _db.cend()) {
            std::cerr << "There is no element with such index." << std::endl;
            return;
        }
        _iter = i;
    } catch (std::logic_error& e) {
        std::cerr << "Invalid index passed to jump." << std::endl;
        return;
    }
}

void CLI::next(const std::string& /*options*/)
{
    if (_db.empty() || _iter == std::prev(_db.cend())) {
        std::cout << "At EOF." << std::endl;
        return;
    }
    _iter = std::next(_iter);
}

void CLI::previous(const std::string& /*options*/)
{
    if (_iter == _db.cbegin()) {
        return;
    }
    _iter = std::prev(_iter);
}

void CLI::help(const std::string& /*options*/)
{
    for (const auto& i : commands) {
        std::cout << i.first.front() << ", " << i.first
            << " - " << i.second.description << std::endl;
    }
}

void CLI::list(const std::string& /*options*/)
{
    for (const auto& i : commands) {
        std::cout << i.first << std::endl;
    }
}

void CLI::quit(const std::string& /*options*/)
{
}

const CLI::map& CLI::getDb() const noexcept
{
    return _db;
}

const CLI::map::const_iterator& CLI::getIterator() const noexcept
{
    return _iter;
}

}
