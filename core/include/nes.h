//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file nes.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief NES system encapsulation.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project Headers.
#include "common.h"
#include "bus.h"
#include "cartridge.h"
#include "drawable.h"

// Language Headers.
#include <memory>
#include <string>
#include <vector>

class OCRNES
{
public:
    Bus bus;
    std::shared_ptr<Cartridge> cart;

    OCRNES() {}

    /**
     * @brief Sets the renderer.
     *
     * @param drawable The renderer.
     */
    void setRenderer(std::shared_ptr<Drawable> drawable)
    {
        bus.ppu.drawable = drawable;
    }

    /**
     * @brief Loads a ROM and resets the emulator.
     *
     * @param path The path to the ROM file.
     * @return true If the load succeeded.
     * @return false If the ROM does not exist.
     */
    bool loadROM(std::string& path)
    {
        cart = std::make_shared<Cartridge>();
        if (cart->load(path))
        {
            bus.insertCartridge(cart);
            bus.reset();
            return true;
        }
        else
            return false;
    }

    /**
     * @brief Creates a savestate at the given filepath.
     */
    void createSaveState(std::string& path)
    {
        std::ofstream state(path, std::ios::out | std::ios::binary);

        if (state.is_open())
        {
            bus.writeSaveStateData(state);
            bus.cpu.writeSaveStateData(state);
            bus.ppu.writeSaveStateData(state);
            bus.cart->writeSaveStateData(state);
        }
    }

    /**
     * @brief Loads a savestate from the given filepath.
     */
    bool loadSaveState(std::string& path)
    {
        std::ifstream state(path, std::ios::in | std::ios::binary);

        if (state.is_open())
        {
            bus.loadSaveStateData(state);
            bus.cpu.loadSaveStateData(state);
            bus.ppu.loadSaveStateData(state);
            bus.cart->loadSaveStateData(state);

            return true;
        }
        else
            return false;
    }
};