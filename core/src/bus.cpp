#include <bus.h>

Bus::Bus()
{
	cart = std::make_shared<Cartridge>();
    // Connect the CPU to the interconnect bus.
    cpu.linkInterconnect(this);;
}

//--------------------------//
// Interconnect Bus R/W		//
//--------------------------//

void Bus::cpuWrite(u16 addr, u8 data)
{
	// Cartridge can stop the write.
    if (cart->cpuWrite(addr, data)) {}
	if (addr >= 0x0000 && addr <= 0x1FFF)
        // System RAM, mirrored every 2KB.
		cpuRAM[addr & 0x07FF] = data;
	else if (addr >= 0x2000 && addr <= 0x3FFF)
        // PPU, mirrored every 8 bytes.
		ppu.cpuWrite(addr & 0x0007, data);
    // APU (not implemented).
	else if ((addr >= 0x4000 && addr <= 0x4013) || addr == 0x4015 || addr == 0x4017) {}
	else if (addr == 0x4014)
	{
        // Writing to this address initiates a DMA transfer.
		dmaPage = data;
		dmaAddr = 0x00;
		dmaInProgress = true;
	}
	else if (addr >= 0x4016 && addr <= 0x4017)
        // Set the internal controller state.
		controllerStateCache[addr & 0x0001] = controller[addr & 0x0001];
}

u8 Bus::cpuRead(u16 addr)
{
    u8 data = 0x00;

    // Cartridge can stop the read.
	if (cart->cpuRead(addr, data)) {}
    else if (addr >= 0x0000 && addr <= 0x1FFF)
		// System RAM, mirrored every 2KB.
		data = cpuRAM[addr & 0x07FF];
	else if (addr >= 0x2000 && addr <= 0x3FFF)
		// PPU, mirrored every 8 bytes.
		data = ppu.cpuRead(addr & 0x0007);
    // APU (not implemented).
	else if (addr == 0x4015) {}
	else if (addr >= 0x4016 && addr <= 0x4017)
	{
		// MSB of the controller status.
		data = (controllerStateCache[addr & 0x0001] & 0x80) > 0;
		controllerStateCache[addr & 0x0001] <<= 1;
	}

	return data;
}

//----------------------//
// System Interface		//
//----------------------//

void Bus::insertCartridge(const std::shared_ptr<Cartridge> &cartridge)
{
	// Connect the cartridge to the interconnect and PPU buses.
	this->cart = cartridge;
	ppu.connectCartridge(cartridge);
}

void Bus::reset()
{
	// Reset the Bus to a known state.
	cart->reset();
	cpu.reset();
	ppu.reset();

	clockCounter = 0;

	dmaPage = 0x00;
	dmaAddr = 0x00;
	dmaData = 0x00;
	dmaIdle = true;
	dmaInProgress = false;
}

void Bus::clockCycle()
{
    // PPU has the fastest clock frequency in the system, so execute a
	// PPU clock cycle every time.
	ppu.clockCycle();

    // CPU runs 3 times slower than PPU.
	if (clockCounter % 3 == 0)
	{
		// DMA transfer in progress?
		if (dmaInProgress)
		{
			// Wait for an even cycle before startig it.
			if (dmaIdle)
			{
				if (clockCounter % 2 == 1)
					dmaIdle = false;
			}
			else
			{
				// Begin DMA.
				if (clockCounter % 2 == 0)
					// Read from CPU bus on even cycles.
					dmaData = cpuRead(dmaPage << 8 | dmaAddr);
				else
				{
					// Write to PPU OAM on odd cycles.
					ppu.publicOAM[dmaAddr] = dmaData;

					// Increment the address LO byte.
					dmaAddr++;

					// If wrapping occurs, 256 bytes have been written,
					// so end DMA.
					if (dmaAddr == 0x00)
					{
						dmaInProgress = false;
						dmaIdle = true;
					}
				}
			}
		}
		else
		{
			// Clock the CPU as normal.
			cpu.clockCycle();
		}
	}

	// The PPU can emit an NMI on VBlank.
	if (ppu.nmi)
	{
		ppu.nmi = false;
		cpu.nmi();
	}

    // Check for cartridge IRQ.
	if (cart->getMapper()->irqState())
	{
		cart->getMapper()->irqClear();
		cpu.irq();
	}

	clockCounter++;
}

//--------------//
//	SaveState	//
//--------------//

void Bus::writeSaveStateData(std::ofstream& state)
{
    // 2048 bytes.
    state.write((char*)&cpuRAM[0], sizeof(u8) * 2048);
    // 4 bytes.
    state.write((char*)&controller[0], sizeof(u8) * 2);
    state.write((char*)&controllerStateCache[0], sizeof(u8) * 2);
    // 4 bytes.
    state.write((char*)&clockCounter, sizeof(u32) * 1);
    // 3 bytes.
    state.write((char*)&dmaPage, sizeof(u8));
    state.write((char*)&dmaAddr, sizeof(u8));
    state.write((char*)&dmaData, sizeof(u8));
    // 2 bytes.
    state.write((char*)&dmaIdle, sizeof(bool));
    state.write((char*)&dmaInProgress, sizeof(bool));
}

void Bus::loadSaveStateData(std::ifstream& state)
{
    // 2048 bytes.
    state.read((char*)&cpuRAM[0], sizeof(u8) * 2048);
    // 4 bytes.
    state.read((char*)&controller[0], sizeof(u8) * 2);
    state.read((char*)&controllerStateCache[0], sizeof(u8) * 2);
    // 4 bytes.
    state.read((char*)&clockCounter, sizeof(u32));
    // 3 bytes.
    state.read((char*)&dmaPage, sizeof(u8));
    state.read((char*)&dmaAddr, sizeof(u8));
    state.read((char*)&dmaData, sizeof(u8));
    // 2 bytes.
    state.read((char*)&dmaIdle, sizeof(bool));
    state.read((char*)&dmaInProgress, sizeof(bool));
}
