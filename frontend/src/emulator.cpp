//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file emurunner.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Controls the running of the emulator instance.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include "emulator.h"

void Emulator::init(Input* input)
{
    this->input = input;
}

bool Emulator::loadROM(std::string& path)
{
    // Create renderer, of which a pointer is held by the PPU.
    renderer = std::make_shared<SFMLRenderer>();

    emu.setRenderer(renderer);
    return emu.loadROM(path);
}

void Emulator::handleControllerInput(u8* controller)
{
    *controller = 0x00;

    // A button.
    *controller |= (input->isKeyPressed(Input::Key::L) && !input->isKeyPressed(Input::Key::LControl)) ? 0x80 : 0x00;
    // B button.
    *controller |= input->isKeyPressed(Input::Key::K) ? 0x40 : 0x00;
    // Select button.
    *controller |= input->isKeyPressed(Input::Key::BackSpace) ? 0x20 : 0x00;
    // Start button.
    *controller |= input->isKeyPressed(Input::Key::Enter) ? 0x10 : 0x00;
    // D-Pad.
    *controller |= input->isKeyPressed(Input::Key::W) ? 0x08 : 0x00;
    *controller |= (input->isKeyPressed(Input::Key::S) && !input->isKeyPressed(Input::Key::LControl)) ? 0x04 : 0x00;
    *controller |= input->isKeyPressed(Input::Key::A) ? 0x02 : 0x00;
    *controller |= input->isKeyPressed(Input::Key::D) ? 0x01 : 0x00;
}

void Emulator::runFrame(bool render)
{
    handleControllerInput(&(emu.bus.controller[0]));

    emu.bus.ppu.renderThisFrame = render;
    do
    {
        emu.bus.clockCycle();
    }
    while(!(emu.bus.ppu.frameComplete));

    emu.bus.ppu.frameComplete = false;
}
