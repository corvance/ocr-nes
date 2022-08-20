//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file gui.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The ImGui GUI class.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include "gui.h"
#include "../app.h"

GUI::GUI(App* app)
{
    this->app = app;
    ImGui::SFML::Init(app->window, false);

    // 1 font size per integer scale of the NES screen.
    ImGui::GetIO().Fonts->AddFontFromFileTTF((app->binaryDir + "Roboto-Medium.ttf").c_str(), 12.0f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF((app->binaryDir + "Roboto-Medium.ttf").c_str(), 24.0f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF((app->binaryDir + "Roboto-Medium.ttf").c_str(), 36.0f);
    ImGui::GetIO().Fonts->AddFontFromFileTTF((app->binaryDir + "Roboto-Medium.ttf").c_str(), 48.0f);

    ImGui::SFML::UpdateFontTexture();
}

void GUI::update(sf::RenderWindow& window, sf::Texture& emuScreen)
{
    ImGui::SFML::Update(window, deltaClock.restart());

    handleInput(window);

    actionWindow(window, emuScreen);
}

void GUI::handleInput(sf::RenderWindow& window)
{
    // Save state.
    if (app->input.isKeyPressed(Input::Key::LControl) && app->input.isKeyDown(Input::Key::S))
    {
        app->saveState(app->stateSlot);
        sendNotification("Saved State " + std::to_string(app->stateSlot), 300);
    }

    // Load state.
    if (app->input.isKeyPressed(Input::Key::LControl) && app->input.isKeyDown(Input::Key::L))
        if (!app->loadState(app->stateSlot))
            sendNotification("No Save In Slot " + std::to_string(app->stateSlot) + "!", 300);

    // Screenshot.
    if (app->input.isKeyDown(Input::Key::F12))
    {
        app->saveScreenshot(window);
        sendNotification("Saved Screenshot", 300);
    }

    // Pause.
    if (app->input.isKeyDown(Input::Key::Esc))
        app->emuPaused = !app->emuPaused;
}

void GUI::actionWindow(sf::RenderWindow& window, sf::Texture& emuScreen)
{
    int font = (u32)(window.getSize().y / 240) - 1;
    if (font < 0) font = 0;
    if (font > 3) font = 3;

    int smallerFont = (u32)(window.getSize().y / 240) - 2;
    if (smallerFont < 0) smallerFont = 0;
    if (smallerFont > 3) smallerFont = 3;

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[font]);

    if (app->emuPaused)
    {
        ImGui::Begin("Emulator Actions", &(app->emuPaused), 0 | ImGuiWindowFlags_NoCollapse
                                        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowSize(ImVec2(window.getSize().x, window.getSize().y));
        ImGui::SetWindowPos(ImVec2(0, 0));

        // Save/load state buttons.
        if (ImGui::Button("Save State"))
            app->saveState(app->stateSlot);

        ImGui::SameLine();

        if (ImGui::Button("Load State"))
            if (!app->loadState(app->stateSlot))
                sendNotification("No Save In Slot " + std::to_string(app->stateSlot) + "!", 300);

        // Keep the following items within the window width including their labels.
        // The label text width is not included in this value.
        ImGui::PushItemWidth(window.getSize().x * 0.5);

        ImGui::InputInt("Save State Slot", &app->stateSlot, 1, 1);
        ImGui::InputInt("Window Scale", &app->windowScale, 1, 1);
        ImGui::SliderInt("Performance Control", &app->frameSkip, 0, 30, "%i");

        // Smaller font for the performance control explanation.
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[smallerFont]);
        ImGui::TextColored(ImVec4(0.0, 1.0, 0.0, 1.0), "Quality +                                           Performance +");
        ImGui::TextColored(ImVec4(1.0, 0.0, 0.0, 1.0), "Performance -                                 Quality -");
        ImGui::PopFont();

        if (ImGui::Button("Save Screenshot On Resume"))
            app->pendingScreenshot = true;

        ImGui::PopItemWidth();

        // Config Save.
        if (ImGui::Button("Save Settings"))
        {
            std::string configPath = app->binaryDir + "config.ini";
            Config::writeConfig(configPath, app);
        }

        ImGui::End();
    }

    // Display all remaining notifications.
    if (notifications.size() > 0)
    {
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[smallerFont]);

        ImGui::Begin("Notifications", NULL, 0 | ImGuiWindowFlags_NoTitleBar
                    | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoResize);
        ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));

        // Set maximum X to half of the screen width, but max out the maximum Y
        // as notifications are capped at 5 at a time anyway.
        ImGui::SetWindowSize(ImVec2(window.getSize().x / 2, window.getSize().y));

        for (int i = 0; i < notifications.size(); i++)
        {
            ImGui::Text(notifications[i].msg.c_str());
            notifications[i].frameDuration--;

            if (notifications[i].frameDuration == 0 || notifications[i].frameDuration >= 36000)
                notifications.erase(notifications.begin() + i);
        }

        ImGui::End();

        ImGui::PopFont();
    }

    ImGui::PopFont();
}

void GUI::sendNotification(const std::string& msg, u32 frameDuration)
{
    notifications.push_back({msg, frameDuration});

    // Max. 5 notifications at any one time.
    if (notifications.size() > 5)
        notifications.erase(notifications.begin());
}