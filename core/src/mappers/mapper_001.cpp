//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper_000.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Mapper 000.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <mappers/mapper_001.h>

Mapper_001::Mapper_001(u8 prgBanks, u8 chrBanks) : Mapper(prgBanks, chrBanks)
{
    vramStatic.resize(32 * 1024);
}

Mapper_001::~Mapper_001() {}

bool Mapper_001::cpuMapRead(u16 addr, u32 &mappedAddr, u8 &data)
{
    if (addr >= 0x6000 && addr <= 0x7FFF)
    {
        // Read is for static cartridge RAM.
        mappedAddr = 0xFFFFFFFF;

        // Perform read.
        data = vramStatic[addr & 0x1FFF];

        return true;
    }

    if (addr >= 0x8000)
    {
        if (controlRegister & 0b01000)
        {
            // 16K mode.
            if (addr >= 0x8000 && addr <= 0xBFFF)
            {
                mappedAddr = prgBankSelect16LO * 0x4000 + (addr & 0x3FFF);
                return true;
            }

            if (addr >= 0xC000 && addr <= 0xFFFF)
            {
                mappedAddr = prgBankSelect16HI * 0x4000 + (addr & 0x3FFF);
                return true;
            }
        }
        else
        {
            // 32KB mode.
            mappedAddr = prgBankSelect32 * 0x8000 + (addr & 0x7FFF);
            return true;
        }
    }

    return false;
}

bool Mapper_001::cpuMapWrite(u16 addr, u32 &mappedAddr, u8 data)
{
    if (addr >= 0x6000 && addr <= 0x7FFF)
    {
        // Write is for static cartridge RAM.
        mappedAddr = 0xFFFFFFFF;

        // Perform write.
        vramStatic[addr & 0x1FFF] = data;

        return true;
    }

    if (addr >= 0x8000)
    {
        if (data & 0x80)
        {
            // Reset serial loading as the MSB is set.
            loadRegister = 0x00;
            loadRegisterCount = 0;
            controlRegister = controlRegister | 0x0C;
        }
        else
        {
            // Serially load data into the load register.
            loadRegister >>= 1;
            loadRegister |= (data & 0x01) << 4;
            loadRegisterCount++;

            if (loadRegisterCount == 5)
            {
                // Get the mapper target register.
                u8 nTargetRegister = (addr >> 13) & 0x03;

                // 0x8000 - 0x9FFF.
                if (nTargetRegister == 0)
                {
                    // Set the control register.
                    controlRegister = loadRegister & 0x1F;

                    switch (controlRegister & 0x03)
                    {
                    case 0:
                        mirrormode = ONESCREEN_LO;
                        break;
                    case 1:
                        mirrormode = ONESCREEN_HI;
                        break;
                    case 2:
                        mirrormode = VERTICAL;
                        break;
                    case 3:
                        mirrormode = HORIZONTAL;
                        break;
                    }
                }
                // 0xA000 - 0xBFFF.
                else if (nTargetRegister == 1)
                {
                    // Set CHR bank LO.
                    if (controlRegister & 0b10000)
                        // 4KB CHR bank.
                        chrBankSelect4LO = loadRegister & 0x1F;
                    else
                        // 8KB CHR bank.
                        chrBankSelect8 = loadRegister & 0x1E;
                }
                // 0xC000 - 0xDFFF.
                else if (nTargetRegister == 2)
                {
                    // Set CHR bank HI.
                    if (controlRegister & 0b10000)
                        // 4KB CHR bank.
                        chrBankSelect4HI = loadRegister & 0x1F;
                }
                // 0xE000 - 0xFFFF.
                else if (nTargetRegister == 3)
                {
                    // Configure PRG Banks
                    u8 prgMode = (controlRegister >> 2) & 0x03;

                    if (prgMode == 0 || prgMode == 1)
                        // Set 32KB PRG bank.
                        prgBankSelect32 = (loadRegister & 0x0E) >> 1;
                    else if (prgMode == 2)
                    {
                        // Fix 16KB PRG bank to the first bank.
                        prgBankSelect16LO = 0;
                        // Set 16KB PRG bank.
                        prgBankSelect16HI = loadRegister & 0x0F;
                    }
                    else if (prgMode == 3)
                    {
                        // Set 16KB PRG bank.
                        prgBankSelect16LO = loadRegister & 0x0F;
                        // Fix 16KB PRG bank to the last bank.
                        prgBankSelect16HI = prgBanks - 1;
                    }
                }

                // Reset the load register.
                loadRegister = 0x00;
                loadRegisterCount = 0;
            }
        }
    }

    return false;
}

bool Mapper_001::ppuMapRead(u16 addr, u32 &mappedAddr)
{
    if (addr < 0x2000)
    {
        if (chrBanks == 0)
        {
            mappedAddr = addr;
            return true;
        }
        else
        {
            if (controlRegister & 0b10000)
            {
                // 4KB CHR bank mode.
                if (addr >= 0x0000 && addr <= 0x0FFF)
                {
                    mappedAddr = chrBankSelect4LO * 0x1000 + (addr & 0x0FFF);
                    return true;
                }

                if (addr >= 0x1000 && addr <= 0x1FFF)
                {
                    mappedAddr = chrBankSelect4HI * 0x1000 + (addr & 0x0FFF);
                    return true;
                }
            }
            else
            {
                // 8KB CHR bank mode.
                mappedAddr = chrBankSelect8 * 0x2000 + (addr & 0x1FFF);
                return true;
            }
        }
    }

    return false;
}

bool Mapper_001::ppuMapWrite(u16 addr, u32 &mappedAddr)
{
    if (addr < 0x2000)
    {
        if (chrBanks == 0)
        {
            mappedAddr = addr;
            return true;
        }

        return true;
    }
    else
        return false;
}

void Mapper_001::reset()
{
    controlRegister = 0x1C;
    loadRegister = 0x00;
    loadRegisterCount = 0x00;

    chrBankSelect4LO = 0;
    chrBankSelect4HI = 0;
    chrBankSelect8 = 0;

    prgBankSelect32 = 0;
    prgBankSelect16LO = 0;
    prgBankSelect16HI = prgBanks - 1;
}

Mirror Mapper_001::mirror()
{
    return mirrormode;
}

//--------------//
// SaveState    //
//--------------//

void Mapper_001::writeSaveStateData(std::ofstream& state)
{
    state.write((char*)&prgBanks, sizeof(u8));
	state.write((char*)&chrBanks, sizeof(u8));

    // 9 bytes.
    state.write((char*)&chrBankSelect4LO, sizeof(u8));
    state.write((char*)&chrBankSelect4HI, sizeof(u8));
    state.write((char*)&chrBankSelect8, sizeof(u8));
    state.write((char*)&prgBankSelect16LO, sizeof(u8));
    state.write((char*)&prgBankSelect16HI, sizeof(u8));
    state.write((char*)&prgBankSelect32, sizeof(u8));
    state.write((char*)&loadRegister, sizeof(u8));
    state.write((char*)&loadRegisterCount, sizeof(u8));
    state.write((char*)&controlRegister, sizeof(u8));

    state.write((char*)&mirrormode, sizeof(Mirror));

    u32 vecLength = vramStatic.size();
    state.write((char*)&vecLength, sizeof(u32));
    state.write((char*)&(vramStatic.data()[0]), vecLength * sizeof(u8));
}

void Mapper_001::loadSaveStateData(std::ifstream& state)
{
    state.read((char*)&prgBanks, sizeof(u8));
	state.read((char*)&chrBanks, sizeof(u8));

    // 9 bytes.
    state.read((char*)&chrBankSelect4LO, sizeof(u8));
    state.read((char*)&chrBankSelect4HI, sizeof(u8));
    state.read((char*)&chrBankSelect8, sizeof(u8));
    state.read((char*)&prgBankSelect16LO, sizeof(u8));
    state.read((char*)&prgBankSelect16HI, sizeof(u8));
    state.read((char*)&prgBankSelect32, sizeof(u8));
    state.read((char*)&loadRegister, sizeof(u8));
    state.read((char*)&loadRegisterCount, sizeof(u8));
    state.read((char*)&controlRegister, sizeof(u8));

    state.read((char*)&mirrormode, sizeof(Mirror));

    u32 vecLength;
    state.read((char*)&vecLength, sizeof(u32));
    vramStatic.resize(vecLength);
    state.read((char*)&(vramStatic.data()[0]), vecLength * sizeof(u8));
}