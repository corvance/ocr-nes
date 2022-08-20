//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file bus.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES interconnect bus.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project Headers.
#include "common.h"
#include "cpu.h"
#include "ppu.h"
#include "cartridge.h"

// Language Headers.
#include <cstdint>
#include <array>

class Bus
{
public:
	Bus();

	//----------//
	// Devices	//
	//----------//

	CPU cpu;
	PPU ppu;
	std::shared_ptr<Cartridge> cart;

	// 2KB of RAM.
	u8 cpuRAM[2048];

	// 2 Controllers.
	u8 controller[2];
	u8 controllerStateCache[2];

	//--------------------------//
    // Interconnect Bus R/W		//
	//--------------------------//

    /**
     * @brief Reads a byte from a memory address.
     *
     * @param addr The address to read from.
     * @return u8 The read byte.
     */
    u8 cpuRead(u16 addr);
    /**
     * @brief Writes a byte to a memory address.
     *
     * @param addr The address to write to.
     * @param data The byte to write.
     */
    void cpuWrite(u16 addr, u8 data);

	//----------------------//
	// System Interface		//
	//----------------------//

    /**
     * @brief Connects a cartridge to the interconnect bus.
     *
     * @param cartridge The cartridge.
     */
    void insertCartridge(const std::shared_ptr<Cartridge>& cartridge);

    /**
     * @brief Resets the system
     */
    void reset();

    /**
     * @brief Executes a clock cycle for the entire system.
     */
    void clockCycle();

    // Used to track what should be clocked on each cycle.
    u32 clockCounter = 0;

private:

    //------//
    // DMA  //
    //------//

	u8 dmaPage = 0x00;
	u8 dmaAddr = 0x00;
	u8 dmaData = 0x00;

    // DMA must start on an even clock cycle, so an idle cycle may be needed.
	bool dmaIdle = true;

    // Flag indicating a DMA transfer is in progress.
	bool dmaInProgress = false;

public:

    //--------------//
    //	SaveState	//
    //--------------//

    /**
     * @brief Writes save state data for the bus to an std::ofstream.
     *
     * @param state The savestate std::ofstream
     */
    void writeSaveStateData(std::ofstream& state);

    /**
     * @brief Loads save state data for the bus from an std::ifstream.
     *
     * @param state The savestate std::ifstream
     */
    void loadSaveStateData(std::ifstream& state);
};
