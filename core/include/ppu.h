//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file ppu.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES game cartridge.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project Headers.
#include "common.h"
#include "cartridge.h"
#include "drawable.h"

// Language Headers.
#include <memory>
#include <cstring>

class PPU
{
public:
	PPU();

	//----------------------------------//
	//	Interconnect Bus Communication	//
	//----------------------------------//

	/**
	 * @brief Reads a byte from the Bus at the given memory address.
	 *
	 * @param addr The address.
	 * @return u8 The read byte.
	 */
	u8 cpuRead(u16 addr);
	/**
	 * @brief Writes a byte to the Bus at the given memory address.
	 *
	 * @param addr The address.
	 * @param data The byte to write.
	 */
	void cpuWrite(u16 addr, u8 data);

	//--------------------------//
	// PPU Bus Communication	//
	//--------------------------//

	/**
	 * @brief Reads a byte from the PPU Bus at the given memory address.
	 *
	 * @param addr The address.
	 * @return u8 The read byte.
	 */
	u8 ppuRead(u16 addr);
	/**
	 * @brief Writes a byte to the PPU Bus at the given memory address.
	 *
	 * @param addr The address.
	 * @param data The byte to write.
	 */
	void ppuWrite(u16 addr, u8 data);

	//------------------//
	// PPU Interface	//
	//------------------//

	/**
	 * @brief Connects the cartridge to the PPU for reading.
	 *
	 * @param cartridge The cartridge.
	 */
	void connectCartridge(const std::shared_ptr<Cartridge> &cartridge);

	/**
	 * @brief Executes 1 PPU clock cycle.
	 */
	void clockCycle();

	/**
	 * @brief Resets the PPU to a known state.
	 */
	void reset();

	bool nmi = false;
	bool scanlineTrigger = false;

	//------------------//
	// Renderer Data    //
	//------------------//

	// All renderers inherit from Drawable, implementing setPixel().
	std::shared_ptr<Drawable> drawable;

	// The screen colour palette.
	RGBAColor palScreen[0x40];

	/**
	 * @brief Gets the screen colour for a palette and pixel.
	 *
	 * @param palette The palette index.
	 * @param pixel The pixel index into the palette.
	 * @return RGBAColor& The RGBAColor screen colour.
	 */
	RGBAColor &getColorFromPalMemory(u8 palette, u8 pixel);

	//----------------------//
	// Emulation Variables	//
	//----------------------//

	// For one-frame-at-a-time emulation.
	bool frameComplete = false;
	// Facilitates frameskip.
	bool renderThisFrame = true;

private:
	//--------------//
	//	PPU Memory	//
	//--------------//

	u8 tblName[2][1024];
	u8 tblPattern[2][4096];
	u8 tblPalette[32];

	//------------------//
	//	PPU Registers	//
	//------------------//

	// The PPU status register, containing its flags.
	union PPUSTATUS
	{
		struct
		{
			u8 unused : 5;
			u8 spriteOverflow : 1;
			u8 spriteZeroHit : 1;
			u8 vBlank : 1;
		};

		u8 reg;
	} status;

	// The PPU mask register, containing rendering flags.
	union PPUMASK
	{
		struct
		{
			u8 greyscale : 1;
			u8 render_background_left : 1;
			u8 renderBGLeft : 1;
			u8 renderBG : 1;
			u8 renderSprites : 1;
			u8 enhanceR : 1;
			u8 enhanceG : 1;
			u8 enhanceB : 1;
		};

		u8 reg;
	} mask;

	// The PPU control register, containing flags and binary values.
	union PPUCTRL
	{
		struct
		{
			u8 nametableX : 1;
			u8 nametableY : 1;
			u8 incMode : 1;
			u8 patternSprite : 1;
			u8 patternBG : 1;
			// 8x8 or 8x16.
			u8 spriteSize : 1;
			// Unused.
			u8 slaveMode : 1;
			u8 enableNMI : 1;
		};

		u8 reg;
	} control;

	union NametablePointerRegister
	{
		struct
		{
			u16 coarseX : 5;
			u16 coarseY : 5;
			u16 nametableX: 1;
			u16 nametableY : 1;
			u16 fineY : 3;
			u16 unused : 1;
		};

		u16 reg = 0x0000;
	};

	// Pointer address into nametable for getting BG tiel info.
	NametablePointerRegister vramAddr;
	// Temporary variable for data to be transferred into vramAddr.
	NametablePointerRegister t_vramAddr;

	// Fine horizontal pixel offset.
	u8 fineX = 0x00;

	// Internal PPU communication.
	u8 addressLatch = 0x00;
	u8 ppuDataBuf = 0x00;

	// Pixel position information.
	s16 scanline = 0;
	s16 cycle = 0;
	bool oddFrame = false;

	//--------------------------//
	// Background Rendering		//
	//--------------------------//

	u8 bgNextTileID = 0x00;
	u8 bgNextTileAttrib = 0x00;
	u8 bgNextTileLSB = 0x00;
	u8 bgNextTileMSB = 0x00;
	u16 bgShifterPatternLO = 0x0000;
	u16 bgShifterPatternHI = 0x0000;
	u16 bgShifterAttribLO = 0x0000;
	u16 bgShifterAttribHI = 0x0000;

	//----------------------//
	// Sprite Rendering		//
	//----------------------//

	struct OAMEntry
	{
		// Sprite Y position.
		u8 y;
		// Pattern memory Tile ID.
		u8 id;
		// Rendering flags.
		u8 attribute;
		// Sprite X position.
		u8 x;
	} OAM[64];

	// Stores the address for manual communication between the CPU and OAM.
	// This is normall avoided in favour of the much faster DMA.
	u8 oamAddr = 0x00;

	OAMEntry spriteScanline[8];

	// Maximum of 8 sprites per scanline.
	u8 spriteCount;
	u8 spriteShifterPatternLO[8];
	u8 spriteShifterPatternHI[8];

	// Sprite Zero-Collision flags.
	bool spriteZeroHitPossible = false;
	bool spriteZeroBeingRendered = false;

	// Cartridge pointer.
	std::shared_ptr<Cartridge> cart;

public:
	// OAM as a u8* for easy byte access during DMA.
	u8 *publicOAM = (u8 *)OAM;

	//------------------//
	// Scanline Actions	//
	//------------------//

	/**
	 * @brief Horiztonally increment the BG tile pointer,
	 */
	void scIncrementScrollX();

	/**
	 * @brief Vertically increment the BG tile pointer.
	 */
	void scIncrementScrollY();

	/**
	 * @brief Transfer the temporary horizontal nametable information store into the real pointer.
	 */
	void scTransferAddressX();

	/**
	 * @brief Transfer the temporary vertical nametable information store into the real pointer.
	 */
	void scTransferAddressY();

	/**
	 * @brief Prime the BG tile shifters for the next tile in the scanline.
	 */
	void scLoadBGShifters();

	/**
	 * @brief Update the pattern and attribute shifters by 1 pixel.
	 */
	void scUpdateShifters();

	/**
	 * @brief Flips a byte.
	 *
	 * @param b The byte.
	 * @return u8 The flipped result.
	 */
	u8 flipByte(u8 b)
	{
		b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
		b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
		b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
		return b;
	}

public:

    //--------------//
    //	SaveState	//
    //--------------//

    /**
     * @brief Writes save state data for the PPU to an std::ofstream.
     *
     * @param state The save state std::ofstream.
     */
    void writeSaveStateData(std::ofstream& state);

    /**
     * @brief Loads save state data for the PPU from an std::ifstream.
     *
     * @param state The save state std::ofstream.
     */
    void loadSaveStateData(std::ifstream& state);
};
