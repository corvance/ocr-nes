//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file cartridge.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES game cartridge.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <cartridge.h>

bool Cartridge::load(const std::string& path)
{
	curPath = path;

	// iNES Format Header.
	struct iNESHeader
	{
		char name[4];
		u8 prgROMChunks;
		u8 chrROMChunks;
		u8 mapper1;
		u8 mapper2;
		u8 prgRAMSize;
		u8 tvSys1;
		u8 tvSys2;
		char unused[5];
	} header;

	std::ifstream ifs(path, std::ifstream::binary);

	if (ifs.is_open())
	{
		// Read the iNES file header.
		ifs.read((char*)&header, sizeof(iNESHeader));

		// Skip unneeded data.
		if (header.mapper1 & 0x04)
			ifs.seekg(512, std::ios_base::cur);

		// Determine the mapper ID.
		mapperID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
		hwMirror = (header.mapper1 & 0x01) ? VERTICAL : HORIZONTAL;

		// There are 3 iNES file types.
		// Ignore 0 and handle 1 and 2.
		u8 iNESFileType = 1;
		if ((header.mapper2 & 0x0C) == 0x08) iNESFileType = 2;

		if (iNESFileType == 1)
		{
			prgBanks = header.prgROMChunks;
			prgMemory.resize(prgBanks * 0x4000);
			ifs.read((char*)prgMemory.data(), prgMemory.size());

			chrBanks = header.chrROMChunks;
			if (chrBanks == 0)
				// Create CHR RAM
				chrMemory.resize(0x2000);
			else
				// Allocate for ROM.
				chrMemory.resize(chrBanks * 0x2000);
			ifs.read((char*)chrMemory.data(), chrMemory.size());
		}
		else if (iNESFileType == 2)
		{
			prgBanks = ((header.prgRAMSize & 0x07) << 8) | header.prgROMChunks;
			prgMemory.resize(prgBanks * 0x4000);
			ifs.read((char*)prgMemory.data(), prgMemory.size());

			chrBanks = ((header.prgRAMSize & 0x38) << 8) | header.chrROMChunks;
			chrMemory.resize(chrBanks * 0x2000);
			ifs.read((char*)chrMemory.data(), chrMemory.size());
		}

		// Load appropriate mapper
		switch (mapperID)
		{
		case 0: mapper = std::make_shared<Mapper_000>(prgBanks, chrBanks); break;
		case 1: mapper = std::make_shared<Mapper_001>(prgBanks, chrBanks); break;
		case 2: mapper = std::make_shared<Mapper_002>(prgBanks, chrBanks); break;
		case 3: mapper = std::make_shared<Mapper_003>(prgBanks, chrBanks); break;
		case 4: mapper = std::make_shared<Mapper_004>(prgBanks, chrBanks); break;
		case 66: mapper = std::make_shared<Mapper_066>(prgBanks, chrBanks); break;
		default: mapper = std::make_shared<Mapper_000>(prgBanks, chrBanks); return false;
		}

		ifs.close();

		return true;
	}
	else
		return false;
}

Cartridge::~Cartridge() {}

//------------------------------//
// Interconnect Bus Linkage     //
//------------------------------//

bool Cartridge::cpuRead(u16 addr, u8& data)
{
	u32 mappedAddr = 0;

	if (mapper->cpuMapRead(addr, mappedAddr, data))
	{
		if (mappedAddr == 0xFFFFFFFF)
			// Mapper has set the data value, e.g., cartridge RAM.
			return true;
		else
			// Mapper has produced an offset into cartridge bank memory.
			data = prgMemory[mappedAddr];

		return true;
	}
	else
		return false;
}

bool Cartridge::cpuWrite(u16 addr, u8 data)
{
	u32 mappedAddr = 0;

	if (mapper->cpuMapWrite(addr, mappedAddr, data))
	{
		if (mappedAddr == 0xFFFFFFFF)
			// Mapper has set the data value, e.g., cartridge RAM.
			return true;
		else
			// Mapper has produced an offset into cartridge bank memory.
			prgMemory[mappedAddr] = data;
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuRead(u16 addr, u8& data)
{
	u32 mappedAddr = 0;

	if (mapper->ppuMapRead(addr, mappedAddr))
	{
		data = chrMemory[mappedAddr];
		return true;
	}
	else
		return false;
}

bool Cartridge::ppuWrite(u16 addr, u8 data)
{
	u32 mappedAddr = 0;

	if (mapper->ppuMapWrite(addr, mappedAddr))
	{
		chrMemory[mappedAddr] = data;
		return true;
	}
	else
		return false;
}

void Cartridge::reset()
{
	// Only resets the mapper.
	if (mapper != nullptr)
		mapper->reset();
}

//--------------//
// Getters      //
//--------------//

Mirror Cartridge::mirror()
{
	Mirror m = mapper->mirror();

	if (m == Mirror::HARDWARE)
		// Mirror configuration is harware-defined.
		return hwMirror;
	else
		// Mirror configuration can be set dynamically by the mapper.
		return m;
}

std::shared_ptr<Mapper> Cartridge::getMapper()
{
	return mapper;
}

u8 Cartridge::getMapperID()
{
	return mapperID;
}


//--------------//
//	SaveState	//
//--------------//

void Cartridge::writeSaveStateData(std::ofstream& state)
{
     // + 1 for null terminator.
     u32 pathLength = curPath.length() + 1;
     state.write((char*)&pathLength, sizeof(u32));
     state.write((char*)curPath.c_str(), pathLength);
     state.write((char*)&hwMirror, sizeof(Mirror));
     // 3 bytes.
     state.write((char*)&mapperID, sizeof(u8));
     state.write((char*)&prgBanks, sizeof(u8));
     state.write((char*)&chrBanks, sizeof(u8));
     u32 vecLength = prgMemory.size();
     state.write((char*)&vecLength, sizeof(u32));
     state.write((char*)&(prgMemory.data()[0]), vecLength);
     vecLength = chrMemory.size();
     state.write((char*)&vecLength, sizeof(u32));
     state.write((char*)&(chrMemory.data()[0]), sizeof(u8) * vecLength);
     mapper->writeSaveStateData(state);
}

void Cartridge::loadSaveStateData(std::ifstream& state)
{
     u32 pathLength;
     state.read((char*)&pathLength, sizeof(u32));
     state.read((char*)curPath.c_str(), pathLength);
     state.read((char*)&hwMirror, sizeof(Mirror));
     state.read((char*)&mapperID, sizeof(u8));
     state.read((char*)&prgBanks, sizeof(u8));
     state.read((char*)&chrBanks, sizeof(u8));
     u32 vecLength;
     state.read((char*)&vecLength, sizeof(u32));
     prgMemory.resize(vecLength);
     state.read((char*)&(prgMemory.data())[0], sizeof(u8) * vecLength);
     state.read((char*)&vecLength, sizeof(u32));
     chrMemory.resize(vecLength);
     state.read((char*)&(chrMemory.data()[0]), sizeof(u8) * vecLength);
     mapper->loadSaveStateData(state);
}