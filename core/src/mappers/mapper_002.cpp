//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper_002.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Mapper 002.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <mappers/mapper_002.h>

Mapper_002::Mapper_002(u8 prgBanks, u8 chrBanks)
    : Mapper(prgBanks, chrBanks) {}

Mapper_002::~Mapper_002() {}

bool Mapper_002::cpuMapRead(u16 addr, u32 &mappedAddr, u8 &data)
{
    if (addr >= 0x8000 && addr <= 0xBFFF)
    {
        mappedAddr = prgBankSelectLO * 0x4000 + (addr & 0x3FFF);
        return true;
    }
    else if (addr >= 0xC000 && addr <= 0xFFFF)
    {
        mappedAddr = prgBankSelectHI * 0x4000 + (addr & 0x3FFF);
        return true;
    }

    return false;
}

bool Mapper_002::cpuMapWrite(u16 addr, u32 &mappedAddr, u8 data)
{
    if (addr >= 0x8000 && addr <= 0xFFFF)
    {
        prgBankSelectLO = data & 0x0F;
    }

    return false;
}

bool Mapper_002::ppuMapRead(u16 addr, u32 &mappedAddr)
{
    if (addr < 0x2000)
    {
        mappedAddr = addr;
        return true;
    }
    else
        return false;
}

bool Mapper_002::ppuMapWrite(u16 addr, u32 &mappedAddr)
{
    // No mapping.
    if (addr < 0x2000 && chrBanks == 0)
    {
        mappedAddr = addr;
        return true;
    }
    return false;
}

void Mapper_002::reset()
{
    prgBankSelectLO = 0;
    prgBankSelectHI = prgBanks - 1;
}

//--------------//
// SaveState    //
//--------------//

void Mapper_002::writeSaveStateData(std::ofstream& state)
{
    state.write((char*)&prgBanks, sizeof(u8));
	state.write((char*)&chrBanks, sizeof(u8));

    // 2 bytes.
    state.write((char*)&prgBankSelectLO, sizeof(u8));
    state.write((char*)&prgBankSelectHI, sizeof(u8));
}

void Mapper_002::loadSaveStateData(std::ifstream& state)
{
    state.read((char*)&prgBanks, sizeof(u8));
	state.read((char*)&chrBanks, sizeof(u8));

    // 2 bytes.
    state.read((char*)&prgBankSelectLO, sizeof(u8));
    state.read((char*)&prgBankSelectHI, sizeof(u8));
}