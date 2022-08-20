//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file input.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Input class capable of differentiating between held and just-pressed keys.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include "input.h"

void Input::update()
{
    // Save the key statuses of the previous frame.
    lastFrameKeys.setMask(thisFrameKeys);

    // Set bits for pressed keys.
    thisFrameKeys.setBit((int)Key::Esc, (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)));
    thisFrameKeys.setBit((int)Key::L, (sf::Keyboard::isKeyPressed(sf::Keyboard::L)));
    thisFrameKeys.setBit((int)Key::K, (sf::Keyboard::isKeyPressed(sf::Keyboard::K)));
    thisFrameKeys.setBit((int)Key::BackSpace, (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace)));
    thisFrameKeys.setBit((int)Key::Enter, (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)));
    thisFrameKeys.setBit((int)Key::W, (sf::Keyboard::isKeyPressed(sf::Keyboard::W)));
    thisFrameKeys.setBit((int)Key::A, (sf::Keyboard::isKeyPressed(sf::Keyboard::A)));
    thisFrameKeys.setBit((int)Key::S, (sf::Keyboard::isKeyPressed(sf::Keyboard::S)));
    thisFrameKeys.setBit((int)Key::D, (sf::Keyboard::isKeyPressed(sf::Keyboard::D)));
    thisFrameKeys.setBit((int)Key::Q, (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)));
    thisFrameKeys.setBit((int)Key::LControl, (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)));
    thisFrameKeys.setBit((int)Key::F12, (sf::Keyboard::isKeyPressed(sf::Keyboard::F12)));
}

bool Input::isKeyPressed(Key keycode)
{
    return thisFrameKeys.getBit((int)keycode);
}

bool Input::isKeyDown(Key keycode)
{
    bool lastFrame = lastFrameKeys.getBit((int)keycode);
    bool thisFrame = thisFrameKeys.getBit((int)keycode);

    return thisFrame && !lastFrame;
}

bool Input::isKeyUp(Key keycode)
{
    bool lastFrame = lastFrameKeys.getBit((int)keycode);
    bool thisFrame = thisFrameKeys.getBit((int)keycode);

    return !thisFrame && lastFrame;
}