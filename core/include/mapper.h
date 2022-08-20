//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES game cartridge mapper base class.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project Headers.
#include "common.h"

enum Mirror
{
	HARDWARE,
	HORIZONTAL,
	VERTICAL,
	ONESCREEN_LO,
	ONESCREEN_HI,
};

class Mapper
{
public:
	Mapper(u8 prgBanks, u8 chrBanks);
	~Mapper();

	//-----------------//
	// R/W Mapping     //
	//-----------------//

	/**
	 * @brief Map an Interconnect Bus read to a PRG ROM offset read.
	 *
	 * @param addr The original read address.
	 * @param mappedAddr u32 to store the mapped address in.
	 * @param data u8 to store the read data in.
	 * @return true Mapping occured.
	 * @return false No mapping occured.
	 */
	virtual bool cpuMapRead(u16 addr, u32& mappedAddr, u8& data) = 0;
	/**
	 * @brief Map an Interconnect Bus write to a PRG ROM offset write.
	 *
	 * @param addr The original write address.
	 * @param mappedAddr u32 to store the mapped address in.
	 * @param data The data to write (default: 0x00).
	 * @return true Mapping occured.
	 * @return false No mapping occured.
	 */
	virtual bool cpuMapWrite(u16 addr, u32& mappedAddr, u8 data = 0) = 0;

	/**
	 * @brief Map a PPU Bus read to a CHR ROM offset read.
	 *
	 * @param addr The original read address.
	 * @param mappedAddr u32 to store the mapped address in.
	 * @return true Mapping occured.
	 * @return false No mapping occured.
	 */
	virtual bool ppuMapRead(u16 addr, u32& mappedAddr) = 0;
	/**
	 * @brief Map a PPU Bus write to a CHR ROM offset write.
	 *
	 * @param addr The original read address.
	 * @param mappedAddr u32 to store the mapped address in.
	 * @return true Mapping occured.
	 * @return false No mapping occured.
	 */
	virtual bool ppuMapWrite(u16 addr, u32& mappedAddr) = 0;

	//------------------------------//
	// Mapper State/Interface		//
	//------------------------------//

	/**
	 * @brief Resets the mapper to a known state.
	 */
	virtual void reset() = 0;

	/**
	 * @brief Gets the mirror mode if it is determined by the mapper.
	 *
	 * @return Mirror The mirror mode.
	 */
	virtual Mirror mirror();

	/**
	 * @brief Gets the IRQ state of the mapper.
	 *
	 * @return true IRQ.
	 * @return false No IRQ.
	 */
	virtual bool irqState();

	/**
	 * @brief Resets the IRQ state of the mapper.
	 */
	virtual void irqClear();

	/**
	 * @brief Perform a per-scanline operation.
	 */
	virtual void scanline();

	//--------------//
	// Save State	//
	//--------------//

	/**
	 * @brief Writes save state data for the mapper to an std::ofstream.
	 *
	 * @param state The save state std::ofstream.
	 */
	virtual void writeSaveStateData(std::ofstream& state) = 0;

	/**
	 * @brief Loads save state data for the mapper from an std::ifstream.
	 *
	 * @param state The save state std::ifstream.
	 */
	virtual void loadSaveStateData(std::ifstream& state) = 0;

protected:
	// Mappers commonly require this information.
	u8 prgBanks = 0;
	u8 chrBanks = 0;
};
