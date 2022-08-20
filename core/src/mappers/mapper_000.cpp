//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper_000.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Mapper 000.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <mappers/mapper_000.h>

Mapper_000::Mapper_000(u8 prgBanks, u8 chrBanks) : Mapper(prgBanks, chrBanks) {}

Mapper_000::~Mapper_000() {}

void Mapper_000::reset() {}

bool Mapper_000::cpuMapRead(u16 addr, u32 &mappedAddr, u8 &data)
{
	// 16KB PRG ROM
	// ->    CPU Bus                  PRG ROM
	// ->    0x8000 -> 0xBFFF: Map    0x0000 -> 0x3FFF
	// ->    0xC000 -> 0xFFFF: Mirror 0x0000 -> 0x3FFF
	// 32KB PRG ROM
	// ->    CPU Address Bus          PRG ROM
	// ->    0x8000 -> 0xFFFF: Map    0x0000 -> 0x7FFF

	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		mappedAddr = addr & (prgBanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool Mapper_000::cpuMapWrite(u16 addr, u32 &mappedAddr, u8 data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		mappedAddr = addr & (prgBanks > 1 ? 0x7FFF : 0x3FFF);
		return true;
	}

	return false;
}

bool Mapper_000::ppuMapRead(u16 addr, u32 &mappedAddr)
{
	// No PPU mapping required.
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		mappedAddr = addr;
		return true;
	}

	return false;
}

bool Mapper_000::ppuMapWrite(u16 addr, u32 &mappedAddr)
{
	if (addr >= 0x0000 && addr <= 0x1FFF)
	{
		// No mapping.
		if (chrBanks == 0)
		{
			mappedAddr = addr;
			return true;
		}
	}

	return false;
}

//--------------//
// SaveState	//
//--------------//

void Mapper_000::writeSaveStateData(std::ofstream& state)
{
	state.write((char*)&prgBanks, sizeof(u8));
	state.write((char*)&chrBanks, sizeof(u8));
}

void Mapper_000::loadSaveStateData(std::ifstream& state)
{
	state.read((char*)&prgBanks, sizeof(u8));
	state.read((char*)&chrBanks, sizeof(u8));
}