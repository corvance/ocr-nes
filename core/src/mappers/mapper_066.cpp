//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper_066.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Mapper 066.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <mappers/mapper_066.h>

Mapper_066::Mapper_066(u8 prgBanks, u8 chrBanks) : Mapper(prgBanks, chrBanks) {}

Mapper_066::~Mapper_066() {}

bool Mapper_066::cpuMapRead(u16 addr, u32& mappedAddr, u8& data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        mappedAddr = prgBankSelect * 0x8000 + (addr & 0x7FFF);
        return true;
    }
    else
        return false;
}

bool Mapper_066::cpuMapWrite(u16 addr, u32& mappedAddr, u8 data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        chrBankSelect = data & 0x03;
        prgBankSelect = (data & 0x30) >> 4;
    }

    return false;
}

bool Mapper_066::ppuMapRead(u16 addr, u32& mappedAddr)
{
    if (addr < 0x2000)
    {
        mappedAddr = chrBankSelect * 0x2000 + addr;
        return true;
    }
    else
        return false;
}

bool Mapper_066::ppuMapWrite(u16 addr, u32 &mappedAddr)
{
    return false;
}

void Mapper_066::reset()
{
    chrBankSelect = 0;
    prgBankSelect = 0;
}

//--------------//
// SaveState    //
//--------------//

void Mapper_066::writeSaveStateData(std::ofstream& state)
{
    state.write((char*)&prgBanks, sizeof(u8));
	state.write((char*)&chrBanks, sizeof(u8));

    // 2 bytes.
    state.write((char*)&chrBankSelect, sizeof(u8));
    state.write((char*)&prgBankSelect, sizeof(u8));
}

void Mapper_066::loadSaveStateData(std::ifstream& state)
{
    state.read((char*)&prgBanks, sizeof(u8));
	state.read((char*)&chrBanks, sizeof(u8));

    // 2 bytes.
    state.read((char*)&chrBankSelect, sizeof(u8));
    state.read((char*)&prgBankSelect, sizeof(u8));
}