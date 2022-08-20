//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file input.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief Input class capable of differentiating between held and just-pressed keys.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

#include <SFML/Graphics.hpp>

#include "bitmask.h"

class Input
{
public:
	/**
	 * @brief Enum of all available keys.
	 *
	 */
	enum class Key {
		None = 0,
        Esc = 1,
		L,
		K,
		BackSpace,
		Enter,
		W,
		A,
		S,
		D,
		LControl,
		Q,
		F12
	};

	/**
	 * @brief Polls keys for this frame and sets them in this frame's bitmask.
	 */
	void update();

	/**
	 * @brief Returns true if the key is pressed (whether it is held or just pressed).
	 *
	 * @param keycode The Input::Key.
	 * @return true If it is pressed.
	 * @return false If it isn't pressed.
	 */
	bool isKeyPressed(Key keycode);

	/**
	 * @brief Returns true if the key has just been pressed (not held).
	 *
	 * @param keycode The Input::Key.
	 * @return true If it has just been pressed.
	 * @return false If it hasn't just been pressed.
	 */
	bool isKeyDown(Key keycode);

	/**
	 * @brief Returns true if the key has just been released.
	 *
	 * @param keycode The Input::Key.
	 * @return true If it has just been released.
	 * @return false If it hasn't just been released.
	 */
	bool isKeyUp(Key keycode);

private:

	// Key statuses for the current frame.
	Bitmask thisFrameKeys;

	// Key statuses for the previous frame, for differentiating held keys.
	Bitmask lastFrameKeys;
};

