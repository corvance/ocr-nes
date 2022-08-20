//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file emurunner.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Controls the running of the emulator instance.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

#include <string>

#include <SFML/Graphics.hpp>

#include <sfml.h>
#include <nes.h>

#include "input/input.h"

class Emulator
{
public:
    Emulator() {}

    void init(Input* input);

    /**
     * @brief Loads a ROM into the emulator instance.
     *
     * @param path The path to the ROM file.
     * @return true If the load succeeded.
     * @return false If the ROM does not exist.
     */
    bool loadROM(std::string& path);

    /**
     * @brief Sets the bits in an NES controller based on the currently pressed controller keys.
     *
     * @param controller Pointer to the controller u8 to handle.
     */
    void handleControllerInput(u8* controller);

    /**
     * @brief Runs a frame of emulation.
     *
     * @param render Whether or not to render this frame.
     */
    void runFrame(bool render);

    // The emulator instance.
    OCRNES emu;

    // The renderer.
    std::shared_ptr<SFMLRenderer> renderer;

    // The input manager.
    Input* input;
};