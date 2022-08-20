//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file drawable.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Virtual base class for rendering targets. All renderers must extend this class and implement setPixel().
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project Headers.
#include "common.h"

/**
 * @brief u32 type representing an RGBA colour, with R, G, B and A union accessors.
 */
union RGBAColor
{
    struct {
        u8 r;
        u8 g;
        u8 b;
        u8 a;
    };

    u32 rgba = 0x000000FF;
};

class Drawable
{
public:
    /**
     * @brief Sets and RGBA pixel in the render target.
     *
     * @param x Pixel X.
     * @param y Pixel Y.
     * @param color RGBA colour to set to.
     */
    virtual void setPixel(int x, int y, RGBAColor color) = 0;
};