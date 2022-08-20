//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file ppu.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES game cartridge.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <ppu.h>

PPU::PPU()
{
	// Initialise the screen palette.

	palScreen[0x00] = {84, 84, 84, 255};
	palScreen[0x01] = {0, 30, 116, 255};
	palScreen[0x02] = {8, 16, 144, 255};
	palScreen[0x03] = {48, 0, 136, 255};
	palScreen[0x04] = {68, 0, 100, 255};
	palScreen[0x05] = {92, 0, 48, 255};
	palScreen[0x06] = {84, 4, 0, 255};
	palScreen[0x07] = {60, 24, 0, 255};
	palScreen[0x08] = {32, 42, 0, 255};
	palScreen[0x09] = {8, 58, 0, 255};
	palScreen[0x0A] = {0, 64, 0, 255};
	palScreen[0x0B] = {0, 60, 0, 255};
	palScreen[0x0C] = {0, 50, 60, 255};
	palScreen[0x0D] = {0, 0, 0, 255};
	palScreen[0x0E] = {0, 0, 0, 255};
	palScreen[0x0F] = {0, 0, 0, 255};

	palScreen[0x10] = {152, 150, 152, 255};
	palScreen[0x11] = {8, 76, 196, 255};
	palScreen[0x12] = {48, 50, 236, 255};
	palScreen[0x13] = {92, 30, 228, 255};
	palScreen[0x14] = {136, 20, 176, 255};
	palScreen[0x15] = {160, 20, 100, 255};
	palScreen[0x16] = {152, 34, 32, 255};
	palScreen[0x17] = {120, 60, 0, 255};
	palScreen[0x18] = {84, 90, 0, 255};
	palScreen[0x19] = {40, 114, 0, 255};
	palScreen[0x1A] = {8, 124, 0, 255};
	palScreen[0x1B] = {0, 118, 40, 255};
	palScreen[0x1C] = {0, 102, 120, 255};
	palScreen[0x1D] = {0, 0, 0, 255};
	palScreen[0x1E] = {0, 0, 0, 255};
	palScreen[0x1F] = {0, 0, 0, 255};

	palScreen[0x20] = {236, 238, 236, 255};
	palScreen[0x21] = {76, 154, 236, 255};
	palScreen[0x22] = {120, 124, 236, 255};
	palScreen[0x23] = {176, 98, 236, 255};
	palScreen[0x24] = {228, 84, 236, 255};
	palScreen[0x25] = {236, 88, 180, 255};
	palScreen[0x26] = {236, 106, 100, 255};
	palScreen[0x27] = {212, 136, 32, 255};
	palScreen[0x28] = {160, 170, 0, 255};
	palScreen[0x29] = {116, 196, 0, 255};
	palScreen[0x2A] = {76, 208, 32, 255};
	palScreen[0x2B] = {56, 204, 108, 255};
	palScreen[0x2C] = {56, 180, 204, 255};
	palScreen[0x2D] = {60, 60, 60, 255};
	palScreen[0x2E] = {0, 0, 0, 255};
	palScreen[0x2F] = {0, 0, 0, 255};

	palScreen[0x30] = {236, 238, 236, 255};
	palScreen[0x31] = {168, 204, 236, 255};
	palScreen[0x32] = {188, 188, 236, 255};
	palScreen[0x33] = {212, 178, 236, 255};
	palScreen[0x34] = {236, 174, 236, 255};
	palScreen[0x35] = {236, 174, 212, 255};
	palScreen[0x36] = {236, 180, 176, 255};
	palScreen[0x37] = {228, 196, 144, 255};
	palScreen[0x38] = {204, 210, 120, 255};
	palScreen[0x39] = {180, 222, 120, 255};
	palScreen[0x3A] = {168, 226, 144, 255};
	palScreen[0x3B] = {152, 226, 180, 255};
	palScreen[0x3C] = {160, 214, 228, 255};
	palScreen[0x3D] = {160, 162, 160, 255};
	palScreen[0x3E] = {0, 0, 0, 255};
	palScreen[0x3F] = {0, 0, 0, 255};
}

//----------------------------------//
//	Interconnect Bus Communication	//
//----------------------------------//

u8 PPU::cpuRead(u16 addr)
{
	u8 data = 0x00;

	switch (addr)
	{
		// Control - Non-readable.
	case 0x0000:
		break;
		// Mask - Non-readable.
	case 0x0001:
		break;
		// Status - Readable.
	case 0x0002:
		// Reading from the status register causes resets.

		// Only the top 3 bits are used. but the others may
		// contain leftover data from the last PPU bus read/write.
		data = (status.reg & 0xE0) | (ppuDataBuf & 0x1F);

		// Clear the VBlank flag and address latch.
		status.vBlank = 0;
		addressLatch = 0;

		break;

		// OAM Address - Non-readable.
	case 0x0003:
		break;

		// OAM Data - Readable.
	case 0x0004:
		data = publicOAM[oamAddr];
		break;

		// Scroll - Non-readable.
	case 0x0005:
		break;

		// PPU Address - Non-readable.
	case 0x0006:
		break;

		// PPU Data - Non-readable.
	case 0x0007:
		// Nametable reads are delayed by one cycle, so use a buffer.
		data = ppuDataBuf;
		ppuDataBuf = ppuRead(vramAddr.reg);

		// Reads in palette memory are not delayed.
		if (vramAddr.reg >= 0x3F00)
			data = ppuDataBuf;

		// PPU reads auto increment the nametable address.
		vramAddr.reg += (control.incMode ? 32 : 1);
		break;
	}

	return data;
}

void PPU::cpuWrite(u16 addr, u8 data)
{
	switch (addr)
	{
		// Control.
	case 0x0000:
		control.reg = data;
		t_vramAddr.nametableX = control.nametableX;
		t_vramAddr.nametableY = control.nametableY;

		break;

		// Mask.
	case 0x0001:
		mask.reg = data;
		break;

		// Status.
	case 0x0002:
		break;

		// OAM Address.
	case 0x0003:
		oamAddr = data;
		break;

		// OAM Data.
	case 0x0004:
		publicOAM[oamAddr] = data;
		break;

		// Scroll.
	case 0x0005:
		if (addressLatch == 0)
		{
			// The first write to the scroll register contains the X offset,
			// to be split into fine and coarse values.
			fineX = data & 0x07;
			t_vramAddr.coarseX = data >> 3;
			addressLatch = 1;
		}
		else
		{
			// The first write to scroll register contains the Y offset,
			// to be split into fine and coarse values.
			t_vramAddr.fineY = data & 0x07;
			t_vramAddr.coarseY = data >> 3;
			addressLatch = 0;
		}
		break;

		// PPU Address.
	case 0x0006:
		if (addressLatch == 0)
		{
			// The fisrt write to this register latches HI byte,
			// the second the LO byte.
			t_vramAddr.reg = (u16)((data & 0x3F) << 8) | (t_vramAddr.reg & 0x00FF);
			addressLatch = 1;
		}
		else
		{
			// Update the VRAM address buffer only after the entire write - it is atomic.
			t_vramAddr.reg = (t_vramAddr.reg & 0xFF00) | data;
			vramAddr = t_vramAddr;
			addressLatch = 0;
		}
		break;

		// PPU Data.
	case 0x0007:
		ppuWrite(vramAddr.reg, data);

		// PPU writes auto increment the nametable address.
		vramAddr.reg += (control.incMode ? 32 : 1);
		break;
	}
}

//--------------------------//
//	PPU Bus Communication	//
//--------------------------//

u8 PPU::ppuRead(u16 addr)
{
	u8 data = 0x00;

	// Adjust address for the PPU.
	addr &= 0x3FFF;

	if (cart->ppuRead(addr, data))
	{
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
		// No mapping.
		data = tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF];
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;

		if (cart->mirror() == Mirror::VERTICAL)
		{
			// Vertical.
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
		else if (cart->mirror() == Mirror::HORIZONTAL)
		{
			// Horizontal.
			if (addr >= 0x0000 && addr <= 0x03FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0400 && addr <= 0x07FF)
				data = tblName[0][addr & 0x03FF];
			if (addr >= 0x0800 && addr <= 0x0BFF)
				data = tblName[1][addr & 0x03FF];
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				data = tblName[1][addr & 0x03FF];
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010)
			addr = 0x0000;
		if (addr == 0x0014)
			addr = 0x0004;
		if (addr == 0x0018)
			addr = 0x0008;
		if (addr == 0x001C)
			addr = 0x000C;

		data = tblPalette[addr] & (mask.greyscale ? 0x30 : 0x3F);
	}

	return data;
}

void PPU::ppuWrite(u16 addr, u8 data)
{
	// Adjust address for the PPU.
	addr &= 0x3FFF;

	if (cart->ppuWrite(addr, data))
	{
	}
	else if (addr >= 0x0000 && addr <= 0x1FFF)
		tblPattern[(addr & 0x1000) >> 12][addr & 0x0FFF] = data;
	else if (addr >= 0x2000 && addr <= 0x3EFF)
	{
		addr &= 0x0FFF;
		if (cart->mirror() == Mirror::VERTICAL)
		{
			// Vertical.
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
		else if (cart->mirror() == Mirror::HORIZONTAL)
		{
			// Horizontal.
			if (addr >= 0x0000 && addr <= 0x03FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0400 && addr <= 0x07FF)
				tblName[0][addr & 0x03FF] = data;
			if (addr >= 0x0800 && addr <= 0x0BFF)
				tblName[1][addr & 0x03FF] = data;
			if (addr >= 0x0C00 && addr <= 0x0FFF)
				tblName[1][addr & 0x03FF] = data;
		}
	}
	else if (addr >= 0x3F00 && addr <= 0x3FFF)
	{
		addr &= 0x001F;
		if (addr == 0x0010)
			addr = 0x0000;
		if (addr == 0x0014)
			addr = 0x0004;
		if (addr == 0x0018)
			addr = 0x0008;
		if (addr == 0x001C)
			addr = 0x000C;

		tblPalette[addr] = data;
	}
}

//------------------//
// Renderer Data    //
//------------------//

RGBAColor &PPU::getColorFromPalMemory(u8 palette, u8 pixel)
{
	// 0x3F00 is the start of palette memory.
	// The palette pointer is shifted by 2 as they are 4 bytes long.
	// The pixel index 0-3 is added to index into the palette.
	// AND with 3F to get an index into the actual array.
	return palScreen[ppuRead(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

//------------------//
// PPU Interface	//
//------------------//

void PPU::connectCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
	this->cart = cartridge;
}

void PPU::reset()
{
	// Reset the PPU to a known state.

	fineX = 0x00;
	addressLatch = 0x00;
	ppuDataBuf = 0x00;
	scanline = 0;
	cycle = 0;

	bgNextTileID = 0x00;
	bgNextTileAttrib = 0x00;
	bgNextTileLSB = 0x00;
	bgNextTileMSB = 0x00;
	bgShifterPatternLO = 0x0000;
	bgShifterPatternHI = 0x0000;
	bgShifterAttribLO = 0x0000;
	bgShifterAttribHI = 0x0000;

	status.reg = 0x00;
	mask.reg = 0x00;
	control.reg = 0x00;
	vramAddr.reg = 0x0000;
	t_vramAddr.reg = 0x0000;
	scanlineTrigger = false;
	oddFrame = false;
}

void PPU::clockCycle()
{
	if (scanline >= -1 && scanline < 240)
	{
		//------------------//
		// BG Rendering.	//
		//------------------//

		if (scanline == 0 && cycle == 0 && oddFrame && (mask.renderBG || mask.renderSprites))
			// Odd frame cycle skip.
			cycle = 1;

		// Scanline -1 is a pre-render scanline, where shifters are configured.
		if (scanline == -1 && cycle == 1)
		{
			// Start of new frame, so end VBlank and clear flags.
			status.vBlank = 0;
			status.spriteOverflow = 0;
			status.spriteZeroHit = 0;

			// Clear Shifters
			for (int i = 0; i < 8; i++)
			{
				spriteShifterPatternLO[i] = 0;
				spriteShifterPatternHI[i] = 0;
			}
		}

		// Visible scanlines.
		if ((cycle >= 2 && cycle < 258) || (cycle >= 321 && cycle < 338))
		{
			scUpdateShifters();

			// Every 2 cycles.
			switch ((cycle - 1) % 8)
			{
			case 0:
				// Load the current BG tile pattern and attributes.
				scLoadBGShifters();

				// Fetch ID of the next BG tile from the nametable address space,
				// masking the 12 relevant bits.
				bgNextTileID = ppuRead(0x2000 | (vramAddr.reg & 0x0FFF));

				break;
			case 2:
				// Fetch the attributes of the next BG tile.
				// The attribute byte specifies 4 distinct palettes.
				// 1 palette applies to 4 tiles, meaning BG tiles share palettes.

				// Fetch from attribute memory, masking the 12 relevant bits.
				bgNextTileAttrib = ppuRead(0x23C0 | (vramAddr.nametableY << 11) | (vramAddr.nametableX << 10) | ((vramAddr.coarseY >> 2) << 3) | (vramAddr.coarseX >> 2));

				// Bottom 2 bits = the selected palettes.
				if (vramAddr.coarseY & 0x02)
					bgNextTileAttrib >>= 4;
				if (vramAddr.coarseX & 0x02)
					bgNextTileAttrib >>= 2;

				bgNextTileAttrib &= 0x03;

				break;
			case 4:
				// Fetch the LSB bit plane of the next BG tile from pattern memory.
				bgNextTileLSB = ppuRead((control.patternBG << 12) + ((u16)bgNextTileID << 4) + (vramAddr.fineY) + 0);
				break;
			case 6:
				// Fetch the next background tile MSB bit plane from the pattern memory
				// This is the same as above, but has a +8 offset to select the next bit plane
				bgNextTileMSB = ppuRead((control.patternBG << 12) + ((u16)bgNextTileID << 4) + (vramAddr.fineY) + 8);
				break;
			case 7:
				// Increment the BG tile pointer horizontally.
				scIncrementScrollX();
				break;
			}
		}

		// End of the visible scanline.
		if (cycle == 256)
			scIncrementScrollY();
		if (cycle == 257)
		{
			scLoadBGShifters();
			scTransferAddressX();
		}
		// Read the next tile ID.
		if (cycle == 338 || cycle == 340)
			bgNextTileID = ppuRead(0x2000 | (vramAddr.reg & 0x0FFF));

		// End of VBlank, so prepare for rendering by resetting Y.
		if (scanline == -1 && cycle >= 280 && cycle < 305)
			scTransferAddressY();

		//------------------//
		// BG Rendering.	//
		//------------------//

		// Perform all sprite evaluation in one hit.
		// May impact compatibility somewhat, but greatly reduces the code needed.
		if (cycle == 257 && scanline >= 0)
		{
			// We've reached the end of a visible scanline. It is now time to determine
			// which sprites are visible on the next scanline, and preload this info
			// into buffers that we can work with while the scanline scans the row.

			// End of visible scanline, so decide the visible sprites for the next.

			// Clear sprite memory.
			std::memset(spriteScanline, 0xFF, 8 * sizeof(OAMEntry));

			// Maximimum of 8 sprites per scanline.
			spriteCount = 0;

			// Reset the shifters.
			for (u8 i = 0; i < 8; i++)
			{
				spriteShifterPatternLO[i] = 0;
				spriteShifterPatternHI[i] = 0;
			}

			// Determine the visible sprites.
			u8 iOAM = 0;
			// Sprite 0 may not exist.
			spriteZeroHitPossible = false;

			while (iOAM < 64 && spriteCount < 9)
			{
				int16_t diff = ((int16_t)scanline - (int16_t)OAM[iOAM].y);

				// If positive difference, scanline may fall within sprite.
				if (diff >= 0 && diff < (control.spriteSize ? 16 : 8) && spriteCount < 8)
				{
					// Sprite is visible.
					if (spriteCount < 8)
					{
						// Sprite zero?
						if (iOAM == 0)
							spriteZeroHitPossible = true;

						memcpy(&spriteScanline[spriteCount], &OAM[iOAM], sizeof(OAMEntry));
					}
					spriteCount++;
				}

				iOAM++;
			}

			status.spriteOverflow = (spriteCount >= 8);

			// The visible sprites are now known and ranked in priority order.
		}

		if (cycle == 340)
		{
			// End of the entire scanline, so prepare the sprite shifters..

			for (u8 i = 0; i < spriteCount; i++)
			{
				// Extract the sprite's 8-bit row patterns.

				u8 spritePatternBitsLO, spritePatternBitsHI;
				u16 spritePatternAddrLO, spritePatternAddrHI;

				// Determine the addresses containing the pattern data byte.
				if (!control.spriteSize)
				{
					// 8x8 Sprite Mode - Pattern table is determined by the Control register.
					if (!(spriteScanline[i].attribute & 0x80))
					{
						// Sprite is no V-flipped.
						spritePatternAddrLO = (control.patternSprite << 12) | (spriteScanline[i].id << 4) | (scanline - spriteScanline[i].y);
					}
					else
					{
						// Sprite is V-flipped.
						spritePatternAddrLO = (control.patternSprite << 12) | (spriteScanline[i].id << 4) | (7 - (scanline - spriteScanline[i].y));
					}
				}
				else
				{
					// 8x16 Sprite Mode - Pattern table is determined by the sprite attribute.
					if (!(spriteScanline[i].attribute & 0x80))
					{
						// Sprite is not v-flipped.
						if (scanline - spriteScanline[i].y < 8)
						{
							// Reading the top half tile.
							spritePatternAddrLO = ((spriteScanline[i].id & 0x01) << 12) | ((spriteScanline[i].id & 0xFE) << 4) | ((scanline - spriteScanline[i].y) & 0x07);
						}
						else
						{
							// Reading the bottom half tile.
							spritePatternAddrLO =
								((spriteScanline[i].id & 0x01) << 12) | (((spriteScanline[i].id & 0xFE) + 1) << 4) | ((scanline - spriteScanline[i].y) & 0x07);
						}
					}
					else
					{
						// Sprite is V-flipped.
						if (scanline - spriteScanline[i].y < 8)
						{
							// Read the top half tile.
							spritePatternAddrLO =
								((spriteScanline[i].id & 0x01) << 12) | (((spriteScanline[i].id & 0xFE) + 1) << 4) | (7 - (scanline - spriteScanline[i].y) & 0x07);
						}
						else
						{
							// Reading the bottom half tile.
							spritePatternAddrLO =
								((spriteScanline[i].id & 0x01) << 12) | ((spriteScanline[i].id & 0xFE) << 4) | (7 - (scanline - spriteScanline[i].y) & 0x07);
						}
					}
				}

				// The HI bit plane equivalent is offset from the LO bit plane by 8 bytes.
				spritePatternAddrHI = spritePatternAddrLO + 8;

				// Read the sprite patterns from the determined addresses.
				spritePatternBitsLO = ppuRead(spritePatternAddrLO);
				spritePatternBitsHI = ppuRead(spritePatternAddrHI);

				// Flip pattern bytes if the sprite is H-flipped.
				if (spriteScanline[i].attribute & 0x40)
				{
					// Flip Patterns Horizontally
					spritePatternBitsLO = flipByte(spritePatternBitsLO);
					spritePatternBitsHI = flipByte(spritePatternBitsHI);
				}

				// Load the pattern into the sprite shift registers, ready to commence rendering.
				spriteShifterPatternLO[i] = spritePatternBitsLO;
				spriteShifterPatternHI[i] = spritePatternBitsHI;
			}
		}
	}

	if (scanline >= 241 && scanline < 261)
	{
		if (scanline == 241 && cycle == 1)
		{
			// End of frame, so begin VBlank.
			status.vBlank = 1;
			// Emit a VBlank NMI.
			if (control.enableNMI)
				nmi = true;
		}
	}

	//----------------------//
	// Pixel Composition	//
	//----------------------//

	// Background.

	// 2-bit pixel.
	u8 bgPixel = 0x00;
	// 3-bit palette index.
	u8 bgPal = 0x00;

	// Only render BGs if BG rendering is enabled.
	if (mask.renderBG)
	{
		if (mask.render_background_left || (cycle >= 9))
		{
			// Select the proper bit according to Fine X.
			u16 bit_mux = 0x8000 >> fineX;

			// Combine to form pixel index
			bgPixel = (((bgShifterPatternHI & bit_mux) > 0) << 1) | ((bgShifterPatternLO & bit_mux) > 0);

			// Get palette.
			bgPal = (((bgShifterAttribHI & bit_mux) > 0) << 1) | (bgShifterAttribLO & bit_mux) > 0;
		}
	}

	// Sprites.

	// 2-bit pixel.
	u8 fgPixel = 0x00;
	// 3-bit palette index.
	u8 fgPal = 0x00;
	u8 fgPriority = 0x00;

	if (mask.renderSprites)
	{
		if (mask.renderBGLeft || (cycle >= 9))
		{
			spriteZeroBeingRendered = false;
			for (u8 i = 0; i < spriteCount; i++)
			{
				// Scanline cycle has collided with the sprite, so the shifters handle it.
				if (spriteScanline[i].x == 0)
				{
					// Determine the value of the pixel.
					u8 fg_pixel_lo = (spriteShifterPatternLO[i] & 0x80) > 0;
					u8 fg_pixel_hi = (spriteShifterPatternHI[i] & 0x80) > 0;
					fgPixel = (fg_pixel_hi << 1) | fg_pixel_lo;

					// Get the palette from the obttom 2 bits.
					fgPal = (spriteScanline[i].attribute & 0x03) + 0x04;
					fgPriority = (spriteScanline[i].attribute & 0x20) == 0;

					// Break to rendering if not transparent.
					if (fgPixel != 0)
					{
						if (i == 0)
							spriteZeroBeingRendered = true;

						break;
					}
				}
			}
		}
	}

	//--------------//
	// Composition	//
	//--------------//

	u8 pixel = 0x00;
	u8 palette = 0x00;

	if (bgPixel == 0 && fgPixel == 0)
	{
		// Both transparent, draw BG colour.
		pixel = 0x00;
		palette = 0x00;
	}
	else if (bgPixel == 0 && fgPixel > 0)
	{
		// FG visible.
		pixel = fgPixel;
		palette = fgPal;
	}
	else if (bgPixel > 0 && fgPixel == 0)
	{
		// BG visible.
		pixel = bgPixel;
		palette = bgPal;
	}
	else if (bgPixel > 0 && fgPixel > 0)
	{
		// Both visible, base on priority.
		if (fgPriority)
		{
			pixel = fgPixel;
			palette = fgPal;
		}
		else
		{
			pixel = bgPixel;
			palette = bgPal;
		}

		// Sprite Zero detection.
		if (spriteZeroHitPossible && spriteZeroBeingRendered)
		{
			// Collision only happens if both BG and sprites are renderer.
			if (mask.renderBG & mask.renderSprites)
			{
				// Left edge control.
				if (!(mask.render_background_left | mask.renderBGLeft))
				{
					if (cycle >= 9 && cycle < 258)
						status.spriteZeroHit = 1;
				}
				else
				{
					if (cycle >= 1 && cycle < 258)
						status.spriteZeroHit = 1;
				}
			}
		}
	}

	// Render to the screen/render target.
	if (renderThisFrame)
		if (scanline >= 0 && scanline < 240)
			drawable->setPixel(cycle - 1, scanline, getColorFromPalMemory(palette, pixel));

	// Progress the renderer.
	cycle++;
	if (mask.renderBG || mask.renderSprites)
		if (cycle == 260 && scanline < 240)
		{
			cart->getMapper()->scanline();
		}

	if (cycle >= 341)
	{
		cycle = 0;
		scanline++;

		if (scanline >= 261)
		{
			scanline = -1;
			frameComplete = true;
			oddFrame = !oddFrame;
		}
	}
}

//------------------//
// Scanline Actions	//
//------------------//

void PPU::scIncrementScrollX()
{
	// Only execute if rendering is enabled.
	if (mask.renderBG || mask.renderSprites)
	{
		// Horizontal incrementing may wrap into another nametable.
		if (vramAddr.coarseX == 31)
		{
			vramAddr.coarseX = 0;
			vramAddr.nametableX = ~vramAddr.nametableX;
		}
		else
			// Increment normally.
			vramAddr.coarseX++;
	}
}

void PPU::scIncrementScrollY()
{
	// Only execute if rendering is enabled.
	if (mask.renderBG || mask.renderSprites)
	{
		// Increment fine Y if within the tile boundary.
		if (vramAddr.fineY < 7)
			vramAddr.fineY++;
		// Exceeded the tile size.
		else
		{
			// Reset the fine Y offset.
			vramAddr.fineY = 0;

			// May wrap into another nametable.
			if (vramAddr.coarseY == 29)
			{
				vramAddr.coarseY = 0;
				vramAddr.nametableY = ~vramAddr.nametableY;
			}
			else if (vramAddr.coarseY == 31)
				vramAddr.coarseY = 0;
			// No wrapping.
			else
				vramAddr.coarseY++;
		}
	}
}

void PPU::scTransferAddressX()
{
	// Only execute if rendering is enabled.
	if (mask.renderBG || mask.renderSprites)
	{
		vramAddr.nametableX = t_vramAddr.nametableX;
		vramAddr.coarseX = t_vramAddr.coarseX;
	}
};

void PPU::scTransferAddressY()
{
	// Only execute if rendering is enabled.
	if (mask.renderBG || mask.renderSprites)
	{
		vramAddr.fineY = t_vramAddr.fineY;
		vramAddr.nametableY = t_vramAddr.nametableY;
		vramAddr.coarseY = t_vramAddr.coarseY;
	}
}

void PPU::scLoadBGShifters()
{
	bgShifterPatternLO = (bgShifterPatternLO & 0xFF00) | bgNextTileLSB;
	bgShifterPatternHI = (bgShifterPatternHI & 0xFF00) | bgNextTileMSB;

	bgShifterAttribLO = (bgShifterAttribLO & 0xFF00) | ((bgNextTileAttrib & 0b01) ? 0xFF : 0x00);
	bgShifterAttribHI = (bgShifterAttribHI & 0xFF00) | ((bgNextTileAttrib & 0b10) ? 0xFF : 0x00);
};

void PPU::scUpdateShifters()
{
	// Only execute if rendering is enabled.
	if (mask.renderBG)
	{
		// BG tile pattern.
		bgShifterPatternLO <<= 1;
		bgShifterPatternHI <<= 1;

		// BG palete attributes.
		bgShifterAttribLO <<= 1;
		bgShifterAttribHI <<= 1;
	}
	if (mask.renderSprites && cycle >= 1 && cycle < 258)
	{
		for (int i = 0; i < spriteCount; i++)
		{
			if (spriteScanline[i].x > 0)
				spriteScanline[i].x--;
			else
			{
				spriteShifterPatternLO[i] <<= 1;
				spriteShifterPatternHI[i] <<= 1;
			}
		}
	}
};

//--------------//
//	SaveState	//
//--------------//

void PPU::writeSaveStateData(std::ofstream &state)
{
	// 3 bytes.
	state.write((char *)&nmi, sizeof(bool));
	state.write((char *)&scanlineTrigger, sizeof(bool));
	state.write((char *)&frameComplete, sizeof(bool));

	// 10272 bytes.
	state.write((char *)&tblName[0], sizeof(u8) * 2 * 1024);
	state.write((char *)&tblPattern[0], sizeof(u8) * 2 * 4096);
	state.write((char *)&tblPalette[0], sizeof(u8) * 32);

	// 3 bytes.
	state.write((char *)&status.reg, sizeof(u8));
	state.write((char *)&mask.reg, sizeof(u8));
	state.write((char *)&control.reg, sizeof(u8));

	// 4 bytes.
	state.write((char *)&vramAddr.reg, sizeof(u16));
	state.write((char *)&t_vramAddr.reg, sizeof(u16));

	// 1 byte.
	state.write((char *)&fineX, sizeof(u8));

	// 7 bytes.
	state.write((char *)&addressLatch, sizeof(u8));
	state.write((char *)&ppuDataBuf, sizeof(u8));
	state.write((char *)&scanline, sizeof(s16));
	state.write((char *)&cycle, sizeof(s16));
	state.write((char *)&oddFrame, sizeof(bool));

	// 12 bytes.
	state.write((char *)&bgNextTileID, sizeof(u8));
	state.write((char *)&bgNextTileAttrib, sizeof(u8));
	state.write((char *)&bgNextTileLSB, sizeof(u8));
	state.write((char *)&bgNextTileMSB, sizeof(u8));
	state.write((char *)&bgShifterPatternLO, sizeof(u16));
	state.write((char *)&bgShifterPatternHI, sizeof(u16));
	state.write((char *)&bgShifterAttribLO, sizeof(u16));
	state.write((char *)&bgShifterAttribHI, sizeof(u16));

	// 256 bytes.
	state.write((char *)&OAM[0], sizeof(OAMEntry) * 64);

	// 7 bytes.
	state.write((char *)&oamAddr, sizeof(u8));
	state.write((char *)&spriteScanline, sizeof(OAMEntry));
	state.write((char *)&spriteCount, sizeof(u8));
	state.write((char *)&spriteShifterPatternLO, sizeof(u8));
	state.write((char *)&spriteShifterPatternHI, sizeof(u8));

	// 2 bytes.
	state.write((char *)&spriteZeroHitPossible, sizeof(bool));
	state.write((char *)&spriteZeroBeingRendered, sizeof(bool));
}

void PPU::loadSaveStateData(std::ifstream &state)
{
	// 3 bytes.
	state.read((char *)&nmi, sizeof(bool));
	state.read((char *)&scanlineTrigger, sizeof(bool));
	state.read((char *)&frameComplete, sizeof(bool));

	// 10272 bytes.
	state.read((char *)&tblName[0], sizeof(u8) * 2 * 1024);
	state.read((char *)&tblPattern[0], sizeof(u8) * 2 * 4096);
	state.read((char *)&tblPalette[0], sizeof(u8) * 32);

	// 3 bytes.
	state.read((char *)&status.reg, sizeof(u8));
	state.read((char *)&mask.reg, sizeof(u8));
	state.read((char *)&control.reg, sizeof(u8));

	// 4 bytes.
	state.read((char *)&vramAddr.reg, sizeof(u16));
	state.read((char *)&t_vramAddr.reg, sizeof(u16));

	// 1 byte.
	state.read((char *)&fineX, sizeof(u8));

	// 7 bytes.
	state.read((char *)&addressLatch, sizeof(u8));
	state.read((char *)&ppuDataBuf, sizeof(u8));
	state.read((char *)&scanline, sizeof(s16));
	state.read((char *)&cycle, sizeof(s16));
	state.read((char *)&oddFrame, sizeof(bool));

	// 12 bytes.
	state.read((char *)&bgNextTileID, sizeof(u8));
	state.read((char *)&bgNextTileAttrib, sizeof(u8));
	state.read((char *)&bgNextTileLSB, sizeof(u8));
	state.read((char *)&bgNextTileMSB, sizeof(u8));
	state.read((char *)&bgShifterPatternLO, sizeof(u16));
	state.read((char *)&bgShifterPatternHI, sizeof(u16));
	state.read((char *)&bgShifterAttribLO, sizeof(u16));
	state.read((char *)&bgShifterAttribHI, sizeof(u16));

	// 256 bytes.
	state.read((char *)&OAM[0], sizeof(OAMEntry) * 64);

	// 7 bytes.
	state.read((char *)&oamAddr, sizeof(u8));
	state.read((char *)&spriteScanline, sizeof(OAMEntry));
	state.read((char *)&spriteCount, sizeof(u8));
	state.read((char *)&spriteShifterPatternLO, sizeof(u8));
	state.read((char *)&spriteShifterPatternHI, sizeof(u8));

	// 2 bytes.
	state.read((char *)&spriteZeroHitPossible, sizeof(bool));
	state.read((char *)&spriteZeroBeingRendered, sizeof(bool));
}
