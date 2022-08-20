//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file cartridge.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES game cartridge.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project Headers.
#include "common.h"
#include "mappers/mapper_000.h"
#include "mappers/mapper_001.h"
#include "mappers/mapper_002.h"
#include "mappers/mapper_003.h"
#include "mappers/mapper_004.h"
#include "mappers/mapper_066.h"

// Language Headers.
#include <memory>
#include <string>
#include <vector>

class Cartridge
{
public:
	Cartridge() {}
	~Cartridge();

	// The path to the currently loaded ROM, used for
	// saving states and screenshots alongside it.
	std::string curPath;

	/**
	 * @brief Loads a ROM.
	 *
	 * @param path The path to the ROM file.
      * @return true If the load succeeded.
      * @return false If the ROM does not exist.
	 */
	bool load(const std::string& path);

	//------------------------------//
	// Interconnect Bus Linkage     //
	//------------------------------//

    /**
     * @brief Reads a byte from a memory address.
     *
     * @param addr The address to read from.
     * @param data The u8 to store the read byte in.
     * @return true Success.
     * @return false Failure.
     */
	bool cpuRead(u16 addr, u8& data);
	/**
     * @brief Writes a byte to a memory address.
     *
     * @param addr The address to write to.
     * @param data The byte to write.
     * @return true Success.
     * @return false Failure.
     */
	bool cpuWrite(u16 addr, u8 data);

    /**
     * @brief Reads a byte from the PPU bus.
     *
     * @param addr The address to read from
     * @param data The u8 to store the read byte in.
     * @return true Success.
     * @return false Failure.
     */
	bool ppuRead(u16 addr, u8& data);
    /**
     * @brief Writes a byte to the PPU bus.
     *
     * @param addr The address to write to.
     * @param data The data to write.
     * @return true Success.
     * @return false Failure.
     */
	bool ppuWrite(u16 addr, u8 data);

	/**
     * @brief Resets the cartridge and mapper to a known state.
     */
	void reset();

	//--------------//
	// Getters      //
	//--------------//

	/**
     * @brief Gets the mapper.
     *
     * @return std::shared_ptr<Mapper> The mapper.
     */
	std::shared_ptr<Mapper> getMapper();

    /**
     * @brief Returns the mirror configuration.
     *
     * @return Mirror The mirror configuration.
     */
	Mirror mirror();

     /**
      * @brief Returns the mapper ID.
      */
     u8 getMapperID();

private:

	//----------------------//
	// Cartridge Internals  //
	//----------------------//

	Mirror hwMirror = HORIZONTAL;

	u8 mapperID = 0;
	u8 prgBanks = 0;
	u8 chrBanks = 0;

	std::vector<u8> prgMemory;
	std::vector<u8> chrMemory;

	std::shared_ptr<Mapper> mapper;

public:
    //--------------//
    // SaveState	//
    //--------------//

     /**
      * @brief Writes save state data for the cartridge and mapper to an std::ofstream.
      *
      * @param state The savestate std::ofstream.
      */
     void writeSaveStateData(std::ofstream& state);

     /**
      * @brief Loads save state data for the cartridge and mapper from an std::ifstream.
      *
      * @param state The savestate std::ifstream.
      */
     void loadSaveStateData(std::ifstream& state);
};
