//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file mapper.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES game cartridge mapper base class virtual implementation.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <mapper.h>

Mapper::Mapper(u8 prgBanks, u8 chrBanks)
{
	this->prgBanks = prgBanks;
	this->chrBanks = chrBanks;

	reset();
}

Mapper::~Mapper() {}

void Mapper::reset() {}

Mirror Mapper::mirror()
{
	return Mirror::HARDWARE;
}

bool Mapper::irqState()
{
	return false;
}

void Mapper::irqClear() {}

void Mapper::scanline() {}

void Mapper::writeSaveStateData(std::ofstream& state) {}

void Mapper::loadSaveStateData(std::ifstream& state) {}