//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file bitmask.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief An input bitmask to facilitate detecting both key holds and single presses.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include "bitmask.h"

Bitmask::Bitmask() : bits(0) {}

void Bitmask::setMask(Bitmask &other)
{
	bits = other.getMask();
}

uint32_t Bitmask::getMask() const
{
	return bits;
}

bool Bitmask::getBit(int position) const
{
	return (bits & (1 << position)) != 0;
}

void Bitmask::setBit(int position, bool on)
{
	if (on)
		setBit(position);
	else
		clearBit(position);
}

void Bitmask::setBit(int position)
{
	bits = bits | 1 << position;
}

void Bitmask::clearBit(int position)
{
	bits = bits & ~(1 << position);
}

void Bitmask::clear()
{
	bits = 0;
}