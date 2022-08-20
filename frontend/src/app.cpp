#include "app.h"

void App::Start(int argc, char** argv)
{
    binaryDir = std::string(argv[0]);
    // Remove the binary file name.
    while (binaryDir[binaryDir.length() - 1] != '/')
        binaryDir.pop_back();


    std::string path;
    // Take the first command line argument as the ROM path.
    if (argc > 1)
        path = std::string(argv[1]);
    else
    {
        printf("No ROM supplied!\n");
        exit(1);
    }

    emulator.init(&input);

    // Load the ROM ready to begin emulation.
    if (!emulator.loadROM(path))
    {
        printf("Game Not Found or Incompatible!\n");
        exit(1);
    }

    GUI gui(this);

    // Create the SFML RenderWindow and resize to be nice and big.
    window.create(sf::VideoMode(256, 240), "OCR NES");
    window.setSize(sf::Vector2u(1024, 960));


    // Measures time taken to process and render, used to determine loop sleep time.
    sf::Clock realClock;
    // Measures frame duration including sleep, used to check 60 FPS consistency.
    sf::Clock sleepClock;
    u32 sleepFPS = 0;

    std::string configPath = binaryDir + "config.ini";
    Config::readConfig(configPath, this);

    while (window.isOpen())
    {
        realClock.restart();

        // Input sanitisation.
        if (stateSlot < 0)
            stateSlot = 0;
        if (windowScale < 1)
            windowScale = 1;
        if (windowScale > 6)
            windowScale = 6;
        if (frameSkip < 0)
            frameSkip = 0;

        if (windowScale != windowScalePrev)
        {
            window.setSize(sf::Vector2u(windowScale * NES_SCREEN_WIDTH, windowScale * NES_SCREEN_HEIGHT));
            windowScalePrev = windowScale;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(window, event);
            if (event.type == sf::Event::Closed)
                window.close();
        }

        input.update();

        // Quit.
        // Here instead of in the GUI to a) allow quitting regardless of GUI errors and
        // b) elegantly quit via a loop break.
        if (input.isKeyPressed(Input::Key::LControl) && input.isKeyPressed(Input::Key::Q))
        {
            window.close();
            break;
        }

        if (pendingScreenshot && !emuPaused)
        {
            pendingScreenshot = false;
            saveScreenshot(window);
        }

        if (frameSkipCounter < 0)
            frameSkipCounter = frameSkip;

        // Run one frame of emulation.
        if (!emuPaused)
        {
            // Only render the last frame.
            emulator.runFrame(frameSkipCounter == 0);
            frameSkipCounter--;
        }

        emulator.renderer->draw(window);

        // Update, declare and render the GUI.
        gui.update(window, emulator.renderer->tex);
        gui.draw(window);

        // Display the window.
        window.display();

        // Maintain 60 FPS.
        if ((1000000 / 60) - realClock.getElapsedTime().asMicroseconds() > 0)
            sf::sleep(sf::microseconds((1000000 / 60) - realClock.getElapsedTime().asMicroseconds()));

        sleepFPS = 1000000 / sleepClock.getElapsedTime().asMicroseconds();
        sleepClock.restart();
    }

    ImGui::SFML::Shutdown();
}

void App::saveState(u8 slot)
{
    std::string statePath = emulator.emu.cart->curPath + ".ocrstate" + std::to_string(slot);
    emulator.emu.createSaveState(statePath);
}

bool App::loadState(u8 slot)
{
    std::string statePath = emulator.emu.cart->curPath + ".ocrstate" + std::to_string(slot);
    return emulator.emu.loadSaveState(statePath);
    return false;
}

void App::saveScreenshot(sf::RenderWindow& window)
{
    std::string path = emulator.emu.cart->curPath + "_SCREENSHOT_" + std::to_string(screenshotID) + ".png";
    emulator.renderer->tex.copyToImage().saveToFile(path);
    screenshotID++;
}
