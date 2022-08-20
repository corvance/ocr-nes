//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file sfml.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief SFML renderer.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

#include <SFML/Graphics.hpp>

#include <drawable.h>

class SFMLRenderer : public Drawable
{
public:
    sf::Texture tex;
    sf::Sprite sprite;

    // The framebuffer.
    sf::Uint8 screen[240][256][4];

    SFMLRenderer()
    {
        // Create the screen texture.
        tex.create(256, 240);

        // Initialise to white.
        for (int y = 0; y < 240; y++)
            for (int x = 0; x < 256; x++)
                for (int component = 0; component < 4; component++)
                    screen[y][x][component] = 255;

        // Update the screen texture with the framebuffer.
        tex.update((sf::Uint8*)screen);

        // Set the screen texture as the screen sprite's texture.
        sprite.setTexture(tex);
    }

    void setPixel(int x, int y, RGBAColor color)
    {
        screen[y][x][0] = color.r;
        screen[y][x][1] = color.g;
        screen[y][x][2] = color.b;
        screen[y][x][3] = color.a;
    }

    void draw(sf::RenderWindow& window)
    {
        // Update the screen sprite's texture with the framebuffer.
        tex.update((sf::Uint8*)screen);

        // Clear the window with black.
        window.clear(sf::Color(0, 0, 0, 255));

        // Draw the screen.
        window.draw(sprite);
    }
};
