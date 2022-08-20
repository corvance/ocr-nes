//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file gui.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The ImGui GUI class.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

#include <vector>

#include <imgui.h>
#include <imgui-SFML.h>

#include <SFML/Graphics.hpp>

#include "../input/input.h"

// Forward declare to avoid circular inclusion.
class App;

class GUI
{
public:
    GUI(App* app);

    /**
     * @brief Declares the GUI.
     */
    void update(sf::RenderWindow& window, sf::Texture& emuScreen);

    /**
     * @brief Handles GUI-related input.
     */
    void handleInput(sf::RenderWindow& window);

    /**
     * @brief Draws the GUI.
     */
    void draw(sf::RenderWindow& window)
    {
        ImGui::SFML::Render(window);
    }

    struct Notification
    {
        std::string msg;
        u32 frameDuration;
    };

    std::vector<Notification> notifications;

    /**
     * @brief Sends a notification to the GUI with the given message and frame duration.
     *
     * @param msg The notification message.
     * @param frameDuration The duration in frames.
     */
    void sendNotification(const std::string& msg, u32 frameDuration);

private:

    //--------------//
    // GUI Items    //
    //--------------//

    /**
     * @brief Declares the main GUI.
     *
     * @param window The RenderWindow to declare it for.
     * @param emuScreen The contents of the emulator output.
     */
    void actionWindow(sf::RenderWindow& window, sf::Texture& emuScreen);

    //------------------//
    // State Variables  //
    //------------------//

    // Initialise ImGui.
    sf::Clock deltaClock;

    App* app;
};