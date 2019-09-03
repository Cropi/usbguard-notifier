#include "ConfigParser.hpp"

#include <iostream>

namespace usbguardNotifier
{
    static const std::vector<std::string> g_nconfig_names = {
        "NotificationPath"
    };

    std::map<std::string, std::string> Notifier::parseConfigFile(std::string file_path)
    {
        std::map<std::string, std::string> config;

        std::ifstream f;
        f.open(file_path, std::ifstream::in);
        if (!f.good())
        {
            std::cerr << "Could not open " << file_path << std::endl;
            // TODO
        }

        std::string line;
        while (std::getline(f, line))
        {
            for(auto key: g_nconfig_names) {

                std::string value;
                if (line.substr(0, key.size()) == key)
                {
                    value = line.substr(key.size()+1);
                    config[key] = value;
                    // std::cout << key << "=" << value << "\n";
                }
            }
        }

        return config;
    }

} /* namespace usbguardNotifier */
