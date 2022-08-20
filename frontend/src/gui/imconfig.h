//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file imconfig.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief ImGui compile-time options.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include <imgui-SFML_export.h>

#define IM_VEC2_CLASS_EXTRA           \
    template<typename T>              \
    ImVec2(const sf::Vector2<T>& v) { \
        x = static_cast<float>(v.x);  \
        y = static_cast<float>(v.y);  \
    }                                 \
                                      \
    template<typename T>              \
    operator sf::Vector2<T>() const { \
        return sf::Vector2<T>(x, y);  \
    }

#define IM_VEC4_CLASS_EXTRA                                                                        \
    ImVec4(const sf::Color& c) : x(c.r / 255.f), y(c.g / 255.f), z(c.b / 255.f), w(c.a / 255.f) {} \
    operator sf::Color() const {                                                                   \
        return sf::Color(static_cast<sf::Uint8>(x * 255.f), static_cast<sf::Uint8>(y * 255.f),     \
                         static_cast<sf::Uint8>(z * 255.f), static_cast<sf::Uint8>(w * 255.f));    \
    }
