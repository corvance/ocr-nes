//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper_004.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Mapper 004.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project Headers.
#include "../mapper.h"

// Language Headers.
#include <vector>

class Mapper_004 : public Mapper
{
public:
	Mapper_004(u8 prgBanks, u8 chrBanks);
	~Mapper_004();

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
	bool cpuMapRead(u16 addr, u32& mappedAddr, u8& data) override;
	/**
	 * @brief Map an Interconnect Bus write to a PRG ROM offset write.
	 *
	 * @param addr The original write address.
	 * @param mappedAddr u32 to store the mapped address in.
	 * @param data The data to write (default: 0x00).
	 * @return true Mapping occured.
	 * @return false No mapping occured.
	 */
	bool cpuMapWrite(u16 addr, u32& mappedAddr, u8 data = 0) override;

	/**
	 * @brief Map a PPU Bus read to a CHR ROM offset read.
	 *
	 * @param addr The original read address.
	 * @param mappedAddr u32 to store the mapped address in.
	 * @return true Mapping occured.
	 * @return false No mapping occured.
	 */
	bool ppuMapRead(u16 addr, u32& mappedAddr) override;
	/**
	 * @brief Map a PPU Bus write to a CHR ROM offset write.
	 *
	 * @param addr The original read address.
	 * @param mappedAddr u32 to store the mapped address in.
	 * @return true Mapping occured.
	 * @return false No mapping occured.
	 */
	bool ppuMapWrite(u16 addr, u32& mappedAddr) override;

	//------------------------------//
	// Mapper State/Interface		//
	//------------------------------//

	/**
	 * @brief Resets the mapper to a known state.
	 */
	void reset() override;

    /**
     * @brief Gets the IRQ state of the mapper.
     */
    bool irqState() override;

    /**
     * @brief Clears the IRQ state of the mapper, stopping any IRQ emission.
     */
    void irqClear() override;

    /**
     * @brief Performs a per-scanline operation.
     */
    void scanline() override;

    /**
     * @brief Gets the mirror configuration of the cartridge, if mapper-controlled.
     */
    Mirror mirror() override;

private:
	u8 targetRegister = 0x00;

	bool prgBankMode = false;
	bool chrInversion = false;

	Mirror mirrorMode = Mirror::HORIZONTAL;

	u32 mapperRegister[8];
	u32 chrBank[8];
	u32 prgBank[4];

	bool irqActive = false;
	bool irqEnable = false;
	bool irqUpdate = false;

	u16 irqCounter = 0x0000;
	u16 irqReload = 0x0000;

	std::vector<u8> vramStatic;

public:

	//--------------//
	// SaveState	//
	//--------------//

	/**
	 * @brief Writes save state data for the mapper to an std::ofstream.
	 *
	 * @param state The save state std::ofstream.
	 */
	void writeSaveStateData(std::ofstream& state) override;

	/**
	 * @brief Loads save state data for the mapper from an std::ifstream.
	 *
	 * @param state The save state std::ifstream.
	 */
	void loadSaveStateData(std::ifstream& state) override;
};