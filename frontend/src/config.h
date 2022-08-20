//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file config.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Writes and loads application configuration from a configuration file.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

#include <map>
#include <string>

// Forwards declare to avoid circular inclusion.

class App;

class Config
{
private:

    /**
     * @brief Checks if a map contains a key.
     *
     * @param searchMap Reference to the map to search.
     * @param key The key to check for.
     * @return true If the key exists.
     * @return false If the key does not exist.
     */
    static bool mapHasKey(std::map<std::string, std::string>& searchMap, std::string key);

public:

    /**
     * @brief Writes the config to local storage.
     *
     * @param path The filepath.
     * @param app Reference to the app instance for which the config must be recorded.
     */
    static void writeConfig(const std::string& path, App* app);

    /**
     * @brief Reads the config from local storage.
     *
     * @param path The filepath.
     * @param app Reference to the app instance for which the config must be read.
     */
    static void readConfig(const std::string& path, App* app);
};