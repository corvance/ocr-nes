//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file config.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Writes and loads application configuration from a configuration file.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include "config.h"
#include "app.h"

bool Config::mapHasKey(std::map<std::string, std::string> &searchMap, std::string key)
{
    return (searchMap.find(key) != searchMap.end());
}

void Config::writeConfig(const std::string &path, App *app)
{
    std::ofstream config(path);

    if (config.is_open())
    {
        config << "StateSlot = " << std::to_string(app->stateSlot) << std::endl;
        config << "WindowScale = " << std::to_string(app->windowScale) << std::endl;
        config << "FrameSkip = " << std::to_string(app->frameSkip) << std::endl;

        config.flush();
        config.close();
    }
}

void Config::readConfig(const std::string &path, App *app)
{
    // Read all config options into a map.
    std::map<std::string, std::string> configEntries;

    std::ifstream config(path);

    if (config.is_open())
    {
        std::string line;
        std::string key;
        std::string value;

        while (std::getline(config, line))
        {
            key = "";
            value = "";

            while (line.length() > 0 && line[0] != '=')
            {
                // Ignore comments.
                if (line[0] == ';')
                    break;

                if (line[0] != ' ')
                    key.push_back(line[0]);
                line.erase(line.begin());
            }

            // Erase the '='.
            if (line.length() > 0)
                if (line[0] != ';')
                    line.erase(line.begin());

            value = "";
            while (line.length() > 0)
            {
                // Ignore comments.
                if (line[0] == ';')
                    break;

                if (line[0] != ' ')
                    value.push_back(line[0]);
                line.erase(line.begin());
            }

            configEntries[key] = value;
        }

        // atoi returns 0 if the parse fails, which works as a default value.
        if (mapHasKey(configEntries, "StateSlot"))
            app->stateSlot = std::atoi(configEntries["StateSlot"].c_str());

        if (mapHasKey(configEntries, "WindowScale"))
            app->windowScale = std::atoi(configEntries["WindowScale"].c_str());

         if (mapHasKey(configEntries, "FrameSkip"))
            app->frameSkip = std::atoi(configEntries["FrameSkip"].c_str());

        config.close();
    }
}