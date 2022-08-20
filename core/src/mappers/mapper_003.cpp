//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper_003.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Mapper 003.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <mappers/mapper_003.h>

Mapper_003::Mapper_003(u8 prgBanks, u8 chrBanks)
: Mapper(prgBanks, chrBanks) {}

Mapper_003::~Mapper_003() {}

bool Mapper_003::cpuMapRead(u16 addr, u32& mappedAddr, u8 &data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
        // 16K ROM.
		if (prgBanks == 1)
			mappedAddr = addr & 0x3FFF;
        // 32K ROM.
		if (prgBanks == 2)
			mappedAddr = addr & 0x7FFF;
		return true;
	}
	else
		return false;
}

bool Mapper_003::cpuMapWrite(u16 addr, u32& mappedAddr, u8 data)
{
	if (addr >= 0x8000 && addr <= 0xFFFF)
	{
		chrBankSelect = data & 0x03;
		mappedAddr = addr;
	}

	return false;
}

bool Mapper_003::ppuMapRead(u16 addr, u32& mappedAddr)
{
	if (addr < 0x2000)
	{
		mappedAddr = chrBankSelect * 0x2000 + addr;
		return true;
	}
	else
		return false;
}

bool Mapper_003::ppuMapWrite(u16 addr, u32& mappedAddr)
{
	return false;
}

void Mapper_003::reset()
{
	chrBankSelect = 0;
}

//--------------//
// SaveState	//
//--------------//

void Mapper_003::writeSaveStateData(std::ofstream& state)
{
	state.write((char*)&prgBanks, sizeof(u8));
	state.write((char*)&chrBanks, sizeof(u8));

    // 1 byte.
    state.write((char*)&chrBankSelect, sizeof(u8));
}

void Mapper_003::loadSaveStateData(std::ifstream& state)
{
	state.read((char*)&prgBanks, sizeof(u8));
	state.read((char*)&chrBanks, sizeof(u8));

    // 1 byte.
    state.read((char*)&chrBankSelect, sizeof(u8));
}