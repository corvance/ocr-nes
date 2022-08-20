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

#include <mappers/mapper_004.h>

Mapper_004::Mapper_004(u8 prgBanks, u8 chrBanks)
    : Mapper(prgBanks, chrBanks)
{
    vramStatic.resize(32 * 1024);
}

Mapper_004::~Mapper_004() {}

bool Mapper_004::cpuMapRead(u16 addr, u32& mappedAddr, u8& data)
{
    if (addr >= 0x6000 && addr <= 0x7FFF)
    {
        // Read is for static cartridge RAM.
        mappedAddr = 0xFFFFFFFF;

        // Perform read.
        data = vramStatic[addr & 0x1FFF];

        return true;
    }
    else if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        mappedAddr = prgBank[0] + (addr & 0x1FFF);
        return true;
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        mappedAddr = prgBank[1] + (addr & 0x1FFF);
        return true;
    }
    else if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        mappedAddr = prgBank[2] + (addr & 0x1FFF);
        return true;
    }
    else if (addr >= 0xE000 && addr <= 0xFFFF)
    {
        mappedAddr = prgBank[3] + (addr & 0x1FFF);
        return true;
    }

    return false;
}

bool Mapper_004::cpuMapWrite(u16 addr, u32& mappedAddr, u8 data)
{
    if (addr >= 0x6000 && addr <= 0x7FFF)
    {
        // Write is for static cartridge RAM.
        mappedAddr = 0xFFFFFFFF;

        // Perform write.
        vramStatic[addr & 0x1FFF] = data;

        return true;
    }
    else if (addr >= 0x8000 && addr <= 0x9FFF)
    {
        // Select bank.
        if (!(addr & 0x0001))
        {
            targetRegister = data & 0x07;
            prgBankMode = (data & 0x40);
            chrInversion = (data & 0x80);
        }
        else
        {
            // Update the target register.
            mapperRegister[targetRegister] = data;

            // Update the pointer table.
            if (chrInversion)
            {
                chrBank[0] = mapperRegister[2] * 0x0400;
                chrBank[1] = mapperRegister[3] * 0x0400;
                chrBank[2] = mapperRegister[4] * 0x0400;
                chrBank[3] = mapperRegister[5] * 0x0400;
                chrBank[4] = (mapperRegister[0] & 0xFE) * 0x0400;
                chrBank[5] = mapperRegister[0] * 0x0400 + 0x0400;
                chrBank[6] = (mapperRegister[1] & 0xFE) * 0x0400;
                chrBank[7] = mapperRegister[1] * 0x0400 + 0x0400;
            }
            else
            {
                chrBank[0] = (mapperRegister[0] & 0xFE) * 0x0400;
                chrBank[1] = mapperRegister[0] * 0x0400 + 0x0400;
                chrBank[2] = (mapperRegister[1] & 0xFE) * 0x0400;
                chrBank[3] = mapperRegister[1] * 0x0400 + 0x0400;
                chrBank[4] = mapperRegister[2] * 0x0400;
                chrBank[5] = mapperRegister[3] * 0x0400;
                chrBank[6] = mapperRegister[4] * 0x0400;
                chrBank[7] = mapperRegister[5] * 0x0400;
            }

            if (prgBankMode)
            {
                prgBank[2] = (mapperRegister[6] & 0x3F) * 0x2000;
                prgBank[0] = (prgBanks * 2 - 2) * 0x2000;
            }
            else
            {
                prgBank[0] = (mapperRegister[6] & 0x3F) * 0x2000;
                prgBank[2] = (prgBanks * 2 - 2) * 0x2000;
            }

            prgBank[1] = (mapperRegister[7] & 0x3F) * 0x2000;
            prgBank[3] = (prgBanks * 2 - 1) * 0x2000;
        }

        return false;
    }
    else if (addr >= 0xA000 && addr <= 0xBFFF)
    {
        if (!(addr & 0x0001))
        {
            // Mirroring.
            if (data & 0x01)
                mirrorMode = Mirror::HORIZONTAL;
            else
                mirrorMode = Mirror::VERTICAL;
        }

        return false;
    }
    else if (addr >= 0xC000 && addr <= 0xDFFF)
    {
        if (!(addr & 0x0001))
            irqReload = data;
        else
            irqCounter = 0x0000;

        return false;
    }
    else if (addr >= 0xE000 && addr <= 0xFFFF)
    {
        if (!(addr & 0x0001))
        {
            irqEnable = false;
            irqActive = false;
        }
        else
            irqEnable = true;

        return false;
    }

    return false;
}

bool Mapper_004::ppuMapRead(u16 addr, u32& mappedAddr)
{
    if (addr >= 0x0000 && addr <= 0x03FF)
    {
        mappedAddr = chrBank[0] + (addr & 0x03FF);
        return true;
    }
    else if (addr >= 0x0400 && addr <= 0x07FF)
    {
        mappedAddr = chrBank[1] + (addr & 0x03FF);
        return true;
    }
    else if (addr >= 0x0800 && addr <= 0x0BFF)
    {
        mappedAddr = chrBank[2] + (addr & 0x03FF);
        return true;
    }
    else if (addr >= 0x0C00 && addr <= 0x0FFF)
    {
        mappedAddr = chrBank[3] + (addr & 0x03FF);
        return true;
    }
    else if (addr >= 0x1000 && addr <= 0x13FF)
    {
        mappedAddr = chrBank[4] + (addr & 0x03FF);
        return true;
    }
    else if (addr >= 0x1400 && addr <= 0x17FF)
    {
        mappedAddr = chrBank[5] + (addr & 0x03FF);
        return true;
    }
    else if (addr >= 0x1800 && addr <= 0x1BFF)
    {
        mappedAddr = chrBank[6] + (addr & 0x03FF);
        return true;
    }
    else if (addr >= 0x1C00 && addr <= 0x1FFF)
    {
        mappedAddr = chrBank[7] + (addr & 0x03FF);
        return true;
    }

    return false;
}

bool Mapper_004::ppuMapWrite(u16 addr, u32& mappedAddr)
{
    return false;
}

void Mapper_004::reset()
{
    targetRegister = 0x00;
    prgBankMode = false;
    chrInversion = false;
    mirrorMode = Mirror::HORIZONTAL;

    irqActive = false;
    irqEnable = false;
    irqUpdate = false;
    irqCounter = 0x0000;
    irqReload = 0x0000;

    for (int i = 0; i < 4; i++)
        prgBank[i] = 0;

    for (int i = 0; i < 8; i++)
    {
        chrBank[i] = 0;
        mapperRegister[i] = 0;
    }

    prgBank[0] = 0 * 0x2000;
    prgBank[1] = 1 * 0x2000;
    prgBank[2] = (prgBanks * 2 - 2) * 0x2000;
    prgBank[3] = (prgBanks * 2 - 1) * 0x2000;
}

bool Mapper_004::irqState()
{
    return irqActive;
}

void Mapper_004::irqClear()
{
    irqActive = false;
}

void Mapper_004::scanline()
{
    if (irqCounter == 0)
        irqCounter = irqReload;
    else
        irqCounter--;

    if (irqCounter == 0 && irqEnable)
        irqActive = true;
}

Mirror Mapper_004::mirror()
{
    return mirrorMode;
}

//--------------//
// SaveState    //
//--------------//

void Mapper_004::writeSaveStateData(std::ofstream& state)
{
	state.write((char*)&prgBanks, sizeof(u8));
	state.write((char*)&chrBanks, sizeof(u8));

    // 1 byte.
    state.write((char*)&targetRegister, sizeof(u8));

    // 2 bytes.
    state.write((char*)&prgBankMode, sizeof(bool));
    state.write((char*)&chrInversion, sizeof(bool));

    state.write((char*)&mirrorMode, sizeof(Mirror));

    // 80 bytes.
    state.write((char*)&mapperRegister[0], sizeof(u32) * 8);
    state.write((char*)&chrBank[0], sizeof(u32) * 8);
    state.write((char*)&prgBank[0], sizeof(u32) * 4);

    // 3 bytes.
    state.write((char*)&irqActive, sizeof(bool));
    state.write((char*)&irqEnable, sizeof(bool));
    state.write((char*)&irqUpdate, sizeof(bool));

    // 4 bytes.
    state.write((char*)&irqCounter, sizeof(u16));
    state.write((char*)&irqReload, sizeof(u16));

    u32 vecLength = vramStatic.size();
    state.write((char*)&vecLength, sizeof(u32));
    state.write((char*)&(vramStatic.data()[0]), vecLength * sizeof(u8));
}

void Mapper_004::loadSaveStateData(std::ifstream& state)
{
	state.read((char*)&prgBanks, sizeof(u8));
	state.read((char*)&chrBanks, sizeof(u8));

        // 1 byte.
    state.read((char*)&targetRegister, sizeof(u8));

    // 2 bytes.
    state.read((char*)&prgBankMode, sizeof(bool));
    state.read((char*)&chrInversion, sizeof(bool));

    state.read((char*)&mirrorMode, sizeof(Mirror));

    // 80 bytes.
    state.read((char*)&mapperRegister[0], sizeof(u32) * 8);
    state.read((char*)&chrBank[0], sizeof(u32) * 8);
    state.read((char*)&prgBank[0], sizeof(u32) * 4);

    // 3 bytes.
    state.read((char*)&irqActive, sizeof(bool));
    state.read((char*)&irqEnable, sizeof(bool));
    state.read((char*)&irqUpdate, sizeof(bool));

    // 4 bytes.
    state.read((char*)&irqCounter, sizeof(u16));
    state.read((char*)&irqReload, sizeof(u16));

    u32 vecLength;
    state.read((char*)&vecLength, sizeof(u32));
    vramStatic.resize(vecLength);
    state.read((char*)&(vramStatic.data()[0]), vecLength * sizeof(u8));
}