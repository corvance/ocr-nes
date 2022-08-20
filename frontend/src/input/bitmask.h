//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file bitmask.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief An input bitmask to facilitate detecting both key holds and single presses.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

#include <common.h>

class Bitmask
{
public:

	Bitmask();

	/**
	 * @brief Overwrites the Bitmask with a new one.
	 * @param other The new bitmask.
	 */
	void setMask(Bitmask& other);

	/**
	 * @brief Return a u32 representing the Bitmask.
	 *
	 * @return u32 The bitmask.
	 */
	u32 getMask() const;

	/**
	 * @brief Tests a bit in the Bitmask.
	 *
	 * @param position THe bit's position.
	 * @return true If the bit is set.
	 * @return false If the bit is unset.
	 */
	bool getBit(int position) const;

	/**
	 * @brief Sets a bit in the Bitmask.
	 *
	 * @param position The bit's position.
	 * @param on The value to set the bit to (1 or 0).
	 */
	void setBit(int position, bool on);

	/**
	 * @brief Sets a bit in the Bitmask to 1.
	 *
	 * @param position The bit's position.
	 */
	void setBit(int position);

	/**
	 * @brief Sets a bit in the Bitmask to 0.
	 *
	 * @param position The bit's position.
	 */
	void clearBit(int position);

	/**
	 * @brief Clears all bits in the bitmask.
	 */
	void clear();

private:
	// The bitmask's integer.
	u32 bits;
};
