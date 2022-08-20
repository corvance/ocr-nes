//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file cpu.cpp
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES 6502-based CPU.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#include <cpu.h>
#include <bus.h>

CPU::CPU()
{
	using x = CPU;
	instructions = {
		{&x::BRK, &x::IMM, 7},
		{&x::ORA, &x::IZX, 6},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 3},
		{&x::ORA, &x::ZP0, 3},
		{&x::ASL, &x::ZP0, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::PHP, &x::IMP, 3},
		{&x::ORA, &x::IMM, 2},
		{&x::ASL, &x::IMP, 2},
		{&x::XXX, &x::IMP, 2},
		{&x::NOP, &x::IMP, 4},
		{&x::ORA, &x::ABS, 4},
		{&x::ASL, &x::ABS, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::BPL, &x::REL, 2},
		{&x::ORA, &x::IZY, 5},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 4},
		{&x::ORA, &x::ZPX, 4},
		{&x::ASL, &x::ZPX, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::CLC, &x::IMP, 2},
		{&x::ORA, &x::ABY, 4},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 7},
		{&x::NOP, &x::IMP, 4},
		{&x::ORA, &x::ABX, 4},
		{&x::ASL, &x::ABX, 7},
		{&x::XXX, &x::IMP, 7},
		{&x::JSR, &x::ABS, 6},
		{&x::AND, &x::IZX, 6},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::BIT, &x::ZP0, 3},
		{&x::AND, &x::ZP0, 3},
		{&x::ROL, &x::ZP0, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::PLP, &x::IMP, 4},
		{&x::AND, &x::IMM, 2},
		{&x::ROL, &x::IMP, 2},
		{&x::XXX, &x::IMP, 2},
		{&x::BIT, &x::ABS, 4},
		{&x::AND, &x::ABS, 4},
		{&x::ROL, &x::ABS, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::BMI, &x::REL, 2},
		{&x::AND, &x::IZY, 5},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 4},
		{&x::AND, &x::ZPX, 4},
		{&x::ROL, &x::ZPX, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::SEC, &x::IMP, 2},
		{&x::AND, &x::ABY, 4},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 7},
		{&x::NOP, &x::IMP, 4},
		{&x::AND, &x::ABX, 4},
		{&x::ROL, &x::ABX, 7},
		{&x::XXX, &x::IMP, 7},
		{&x::RTI, &x::IMP, 6},
		{&x::EOR, &x::IZX, 6},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 3},
		{&x::EOR, &x::ZP0, 3},
		{&x::LSR, &x::ZP0, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::PHA, &x::IMP, 3},
		{&x::EOR, &x::IMM, 2},
		{&x::LSR, &x::IMP, 2},
		{&x::XXX, &x::IMP, 2},
		{&x::JMP, &x::ABS, 3},
		{&x::EOR, &x::ABS, 4},
		{&x::LSR, &x::ABS, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::BVC, &x::REL, 2},
		{&x::EOR, &x::IZY, 5},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 4},
		{&x::EOR, &x::ZPX, 4},
		{&x::LSR, &x::ZPX, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::CLI, &x::IMP, 2},
		{&x::EOR, &x::ABY, 4},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 7},
		{&x::NOP, &x::IMP, 4},
		{&x::EOR, &x::ABX, 4},
		{&x::LSR, &x::ABX, 7},
		{&x::XXX, &x::IMP, 7},
		{&x::RTS, &x::IMP, 6},
		{&x::XXX, &x::IMP, 2},
		{&x::ADC, &x::IZX, 6},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 3},
		{&x::ADC, &x::ZP0, 3},
		{&x::ROR, &x::ZP0, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::PLA, &x::IMP, 4},
		{&x::ADC, &x::IMM, 2},
		{&x::ROR, &x::IMP, 2},
		{&x::XXX, &x::IMP, 2},
		{&x::JMP, &x::IND, 5},
		{&x::ADC, &x::ABS, 4},
		{&x::ROR, &x::ABS, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::BVS, &x::REL, 2},
		{&x::ADC, &x::IZY, 5},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 4},
		{&x::ADC, &x::ZPX, 4},
		{&x::ROR, &x::ZPX, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::SEI, &x::IMP, 2},
		{&x::ADC, &x::ABY, 4},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 7},
		{&x::NOP, &x::IMP, 4},
		{&x::ADC, &x::ABX, 4},
		{&x::ROR, &x::ABX, 7},
		{&x::XXX, &x::IMP, 7},
		{&x::NOP, &x::IMP, 2},
		{&x::STA, &x::IZX, 6},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 6},
		{&x::STY, &x::ZP0, 3},
		{&x::STA, &x::ZP0, 3},
		{&x::STX, &x::ZP0, 3},
		{&x::XXX, &x::IMP, 3},
		{&x::DEY, &x::IMP, 2},
		{&x::NOP, &x::IMP, 2},
		{&x::TXA, &x::IMP, 2},
		{&x::XXX, &x::IMP, 2},
		{&x::STY, &x::ABS, 4},
		{&x::STA, &x::ABS, 4},
		{&x::STX, &x::ABS, 4},
		{&x::XXX, &x::IMP, 4},
		{&x::BCC, &x::REL, 2},
		{&x::STA, &x::IZY, 6},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 6},
		{&x::STY, &x::ZPX, 4},
		{&x::STA, &x::ZPX, 4},
		{&x::STX, &x::ZPY, 4},
		{&x::XXX, &x::IMP, 4},
		{&x::TYA, &x::IMP, 2},
		{&x::STA, &x::ABY, 5},
		{&x::TXS, &x::IMP, 2},
		{&x::XXX, &x::IMP, 5},
		{&x::NOP, &x::IMP, 5},
		{&x::STA, &x::ABX, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::LDY, &x::IMM, 2},
		{&x::LDA, &x::IZX, 6},
		{&x::LDX, &x::IMM, 2},
		{&x::XXX, &x::IMP, 6},
		{&x::LDY, &x::ZP0, 3},
		{&x::LDA, &x::ZP0, 3},
		{&x::LDX, &x::ZP0, 3},
		{&x::XXX, &x::IMP, 3},
		{&x::TAY, &x::IMP, 2},
		{&x::LDA, &x::IMM, 2},
		{&x::TAX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 2},
		{&x::LDY, &x::ABS, 4},
		{&x::LDA, &x::ABS, 4},
		{&x::LDX, &x::ABS, 4},
		{&x::XXX, &x::IMP, 4},
		{&x::BCS, &x::REL, 2},
		{&x::LDA, &x::IZY, 5},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 5},
		{&x::LDY, &x::ZPX, 4},
		{&x::LDA, &x::ZPX, 4},
		{&x::LDX, &x::ZPY, 4},
		{&x::XXX, &x::IMP, 4},
		{&x::CLV, &x::IMP, 2},
		{&x::LDA, &x::ABY, 4},
		{&x::TSX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 4},
		{&x::LDY, &x::ABX, 4},
		{&x::LDA, &x::ABX, 4},
		{&x::LDX, &x::ABY, 4},
		{&x::XXX, &x::IMP, 4},
		{&x::CPY, &x::IMM, 2},
		{&x::CMP, &x::IZX, 6},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::CPY, &x::ZP0, 3},
		{&x::CMP, &x::ZP0, 3},
		{&x::DEC, &x::ZP0, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::INY, &x::IMP, 2},
		{&x::CMP, &x::IMM, 2},
		{&x::DEX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 2},
		{&x::CPY, &x::ABS, 4},
		{&x::CMP, &x::ABS, 4},
		{&x::DEC, &x::ABS, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::BNE, &x::REL, 2},
		{&x::CMP, &x::IZY, 5},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 4},
		{&x::CMP, &x::ZPX, 4},
		{&x::DEC, &x::ZPX, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::CLD, &x::IMP, 2},
		{&x::CMP, &x::ABY, 4},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 7},
		{&x::NOP, &x::IMP, 4},
		{&x::CMP, &x::ABX, 4},
		{&x::DEC, &x::ABX, 7},
		{&x::XXX, &x::IMP, 7},
		{&x::CPX, &x::IMM, 2},
		{&x::SBC, &x::IZX, 6},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::CPX, &x::ZP0, 3},
		{&x::SBC, &x::ZP0, 3},
		{&x::INC, &x::ZP0, 5},
		{&x::XXX, &x::IMP, 5},
		{&x::INX, &x::IMP, 2},
		{&x::SBC, &x::IMM, 2},
		{&x::NOP, &x::IMP, 2},
		{&x::SBC, &x::IMP, 2},
		{&x::CPX, &x::ABS, 4},
		{&x::SBC, &x::ABS, 4},
		{&x::INC, &x::ABS, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::BEQ, &x::REL, 2},
		{&x::SBC, &x::IZY, 5},
		{&x::XXX, &x::IMP, 2},
		{&x::XXX, &x::IMP, 8},
		{&x::NOP, &x::IMP, 4},
		{&x::SBC, &x::ZPX, 4},
		{&x::INC, &x::ZPX, 6},
		{&x::XXX, &x::IMP, 6},
		{&x::SED, &x::IMP, 2},
		{&x::SBC, &x::ABY, 4},
		{&x::NOP, &x::IMP, 2},
		{&x::XXX, &x::IMP, 7},
		{&x::NOP, &x::IMP, 4},
		{&x::SBC, &x::ABX, 4},
		{&x::INC, &x::ABX, 7},
		{&x::XXX, &x::IMP, 7},
	};
}

//--------------//
// Execution	//
//--------------//

void CPU::reset()
{
	// Read the new PC from 0xFFFC.
	addrAbs = 0xFFFC;
	pc = ((u16)read(addrAbs + 1) << 8) | read(addrAbs + 0);

	// Reset registers.
	a = 0;
	x = 0;
	y = 0;
	sp = 0xFD;
	status = 0x00;
	setFlag(U, true);

	// Reset emulation variables.
	addrRel = 0x0000;
	addrAbs = 0x0000;
	fetched = 0x00;

	// A CPU reset takes 8 cycles.
	instrCycles = 8;
}

void CPU::irq()
{
	// Only service the interrupt if they are enabled.
	if (getFlag(I) == 0)
	{
		// Push PC to the stack.
		write(0x0100 + sp, (pc >> 8) & 0x00FF);
		sp--;
		write(0x0100 + sp, pc & 0x00FF);
		sp--;

		// Push status register to the stack.
		setFlag(B, 0);
		setFlag(U, 1);
		setFlag(I, 1);
		write(0x0100 + sp, status);
		sp--;

		// Read new PC from 0xFFFE.
		addrAbs = 0xFFFE;
		pc = ((u16)read(addrAbs + 1) << 8) | read(addrAbs + 0);

		// Servicing an IRQ takes 7 cycles.
		instrCycles = 7;
	}
}

void CPU::nmi()
{
	// Push PC to the stack.
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	// Push status register to the stack, with
	// the Break, Unused and Interrupt Disable flags set.
	setFlag(B, 0);
	setFlag(U, 1);
	setFlag(I, 1);
	write(0x0100 + sp, status);
	sp--;

	// Read new PC from 0xFFFA.
	addrAbs = 0xFFFA;
	pc = ((u16)read(addrAbs + 1) << 8) | read(addrAbs + 0);

	// Servicing an NMI takes 7 cycles.
	instrCycles = 8;
}

void CPU::clockCycle()
{
	// This implementation performs all of an instruction's execution in
	// its first cycle, rather than as multiple microcode operations
	// over multiple cycles. This may reduce compatibility, but timing
	// is kept correct by idling for the remaining number of cycles.
	if (instrCycles == 0)
	{
		// Read the opcode of the next instruction.
		curOpcode = read(pc);
		pc++;

		// Unused flag is always set.
		setFlag(U, 1);

		// Set the remaining cycles to the instruction's
		// base number of cycles.
		instrCycles = instructions[curOpcode].cycles;

		// The addressing mode and instruction functions
		// both return u8's which can be logiclly ANDed together
		// to give the number of extra cycles needed, if any.

		// Fetch data.
		u8 extra1 = (this->*instructions[curOpcode].addrmode)();
		// Execute the instruction.
		u8 extra2 = (this->*instructions[curOpcode].impl)();

		// Update the remaining cycles with the extra cycles needed.
		instrCycles += (extra1 & extra2);

		// Unused flag is always set.
		setFlag(U, 1);
	}

	// Decrement the remaining cycles for this instruction.
	instrCycles--;
}

//------------------//
// Flag Operations	//
//------------------//

u8 CPU::getFlag(Flag f)
{
	return ((status & f) > 0) ? 1 : 0;
}

void CPU::setFlag(Flag f, bool v)
{
	status = v ? (status | f) : (status & ~f);
}

//--------------------------------//
// Interconnect Linkage	(Private) //
//--------------------------------//

u8 CPU::read(u16 a)
{
	return bus->cpuRead(a);
}

void CPU::write(u16 a, u8 d)
{
	bus->cpuWrite(a, d);
}

u8 CPU::fetch()
{
	if (!(instructions[curOpcode].addrmode == &CPU::IMP))
		fetched = read(addrAbs);
	return fetched;
}

//----------------------//
// Addressing Modes     //
//----------------------//

u8 CPU::IMP()
{
	// No extra data needed except possibly the accumulator contents.
	fetched = a;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::IMM()
{
	// The data is an operand of the instruction, so advance the PC
	// adn set it as the data address.
	addrAbs = pc++;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::ZP0()
{
	// The instruction operand is an offset into the Zero Page.
	addrAbs = read(pc) & 0x00FF;
	pc++;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::ZPX()
{
	// The instruction operand is an offset into the Zero Page.
	// Offset further by the contents of X.
	addrAbs = (read(pc) + x) & 0x00FF;
	pc++;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::ZPY()
{
	// The instruction operand is an offset into the Zero Page.
	// Offset further by the contents of X.
	addrAbs = (read(pc) + y) & 0x00FF;
	pc++;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::REL()
{
	// Address is a signed 8-bit relative offset from the PC.
	addrRel = read(pc);
	pc++;

	// OR the read unsigned address with 0xFF00 if negative.
	// This allows it to be kept unsigned and added to have the
	// effect of a negative 8-bit signed offset.
	if (addrRel & 0x80)
		addrRel |= 0xFF00;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::ABS()
{
	// Read an absolute u16 address from which the
	// instruction will fetch data.
	addrAbs = ((u16)read(pc + 1) << 8) | read(pc);
	pc += 2;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::ABX()
{
	// Read an absolute u16 address and offset by X.
	u16 page = read(pc + 1);
	addrAbs = ((page << 8) | read(pc)) + x;
	pc += 2;

	// If the X offset changes the page, an extra clock
	// cycle is needed.
	return ((addrAbs & 0xFF00) != (page << 8)) ? 1 : 0;
}

u8 CPU::ABY()
{
	// Read an absolute u16 address and offset by Y.
	u16 page = read(pc + 1);
	addrAbs = ((page << 8) | read(pc)) + y;
	pc += 2;

	// If the Y offset changes the page, an extra clock
	// cycle is needed.
	return ((addrAbs & 0xFF00) != (page << 8)) ? 1 : 0;
}

u8 CPU::IND()
{
	u16 lo = read(pc);
	u16 ptr = ((u16)read(pc + 1) << 8) | lo;
	pc += 2;

	// Hardware Bug.
	// If the low byte of the pointer address is 0xFF,
	// reading the high byte of the final address will cause
	// a page change. The hardware doesn't do this correctly,
	// instead wrapping to the start of the page.
	if (lo == 0x00FF)
		addrAbs = (read(ptr & 0xFF00) << 8) | read(ptr);
	// No bug.
	else
		addrAbs = (read(ptr + 1) << 8) | read(ptr);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::IZX()
{
	// Read the u8 Zero Page offset at PC.
	u16 offset = read(pc);
	pc++;

	// The absolute address is read from the Zero Page
	// at the read offset, offset by X.
	addrAbs = ((u16)read((u16)(offset + (u16)x) & 0x00FF) << 8)
			| read((u16)(offset + (u16)x + 1) & 0x00FF);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::IZY()
{
	// Read the u8 Zero Page offset at PC.
	u16 offset = read(pc);
	pc++;

	// The absolute address is read from the Zero Page
	// at the read offset, and the read absolute address
	// is then offset by the contents of Y.
	u16 page = read((offset + 1) & 0x00FF);
	addrAbs = ((page << 8) | read(offset & 0x00FF)) + y;

	return ((addrAbs & 0xFF00) != (page << 8)) ? 1 : 0;
}

//------------------//
// Instructions     //
//------------------//

u8 CPU::ADC()
{
	fetch();

	// u16 addition space for detecting carry.
	resBuf = (u16)a + (u16)fetched + (u16)getFlag(C);

	// Set flags.
	setFlag(C, resBuf > 255);
	setFlag(Z, (resBuf & 0x00FF) == 0);
	setFlag(V, (~((u16)a ^ (u16)fetched) & ((u16)a ^ (u16)resBuf)) & 0x0080);
	setFlag(N, resBuf & 0x80);

	// Store result in the accumulator.
	a = resBuf & 0x00FF;

	// May require an additional clock cycle.
	return 1;
}

u8 CPU::AND()
{
	fetch();

	// Perform logical AND.
	a = a & fetched;

	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);

	// May require an additional clock cycle.
	return 1;
}

u8 CPU::ASL()
{
	fetch();

	// Perform Arithmetic Shift Left.
	resBuf = (u16)fetched << 1;

	// Set flags.
	setFlag(C, (resBuf & 0xFF00) > 0);
	setFlag(Z, (resBuf & 0x00FF) == 0x00);
	setFlag(N, resBuf & 0x80);

	// Write the result to the appropriate destination.
	if (instructions[curOpcode].addrmode == &CPU::IMP)
		a = resBuf & 0x00FF;
	else
		write(addrAbs, resBuf & 0x00FF);

	// Won't require an additional clock cycle.
	return 0;
}

u8 CPU::BCC()
{
	return conditionalBranch(getFlag(C) == 0);
}

u8 CPU::BCS()
{
	return conditionalBranch(getFlag(C) == 1);
}

u8 CPU::BEQ()
{
	return conditionalBranch(getFlag(Z) == 1);
}

u8 CPU::BIT()
{
	fetch();

	// Perform logical AND for bit test.
	resBuf = a & fetched;

	// Set flags according to the result bits.
	setFlag(Z, (resBuf & 0x00FF) == 0x00);
	setFlag(N, fetched & (1 << 7));
	setFlag(V, fetched & (1 << 6));

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::BMI()
{
	return conditionalBranch(getFlag(N) == 1);
}

u8 CPU::BNE()
{
	return conditionalBranch(getFlag(Z) == 0);
}

u8 CPU::BPL()
{
	return conditionalBranch(getFlag(N) == 0);
}

u8 CPU::BRK()
{
	pc++;

	// Push PC to the stack.
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	// Push status register to the stack, with
	// the Interrupt Disable and Break flags set.
	setFlag(I, 1);
	setFlag(B, 1);
	write(0x0100 + sp, status);
	sp--;

	// Unset the break flag.
	setFlag(B, 0);

	// Read the new PC from oxFFFE.
	pc = ((u16)read(0xFFFF) << 8) | read(0xFFFE);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::BVC()
{
	return conditionalBranch(getFlag(V) == 0);
}

u8 CPU::BVS()
{
	return conditionalBranch(getFlag(V) == 1);
}

u8 CPU::CLC()
{
	// Perform Carry Flag clear.
	setFlag(C, 0);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::CLD()
{
	// Perform decimal flag clear.
	setFlag(D, 0);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::CLI()
{
	// Perform Disable Interrupts flag clear.
	setFlag(I, 0);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::CLV()
{
	// Perform Overflow flag clear.
	setFlag(V, 0);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::CMP()
{
	fetch();

	// Perform accumulator comparison.
	resBuf = (u16)a - (u16)fetched;

	// Set flags.
	setFlag(C, a >= fetched);
	setFlag(Z, (resBuf & 0x00FF) == 0x0000);
	setFlag(N, resBuf & 0x0080);

	// May require an additional clock cycle.
	return 1;
}

u8 CPU::CPX()
{
	fetch();

	// Perform X register comparison.
	resBuf = (u16)x - (u16)fetched;

	// Set flags.
	setFlag(C, x >= fetched);
	setFlag(Z, (resBuf & 0x00FF) == 0x0000);
	setFlag(N, resBuf & 0x0080);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::CPY()
{
	fetch();

	// Perform Y register comparison.
	resBuf = (u16)y - (u16)fetched;

	// Set flags.
	setFlag(C, y >= fetched);
	setFlag(Z, (resBuf & 0x00FF) == 0x0000);
	setFlag(N, resBuf & 0x0080);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::DEC()
{
	fetch();

	// Perform decrement on memory location.
	resBuf = fetched - 1;
	write(addrAbs, resBuf & 0x00FF);

	// Set flags.
	setFlag(Z, (resBuf & 0x00FF) == 0x0000);
	setFlag(N, resBuf & 0x0080);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::DEX()
{
	// Perform decrement on X register.
	x--;

	// Set flags.
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::DEY()
{
	// Perform decrement on Y register.
	y--;

	// Set flags.
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::EOR()
{
	fetch();

	// Perform logical XOR.
	a = a ^ fetched;

	// Set flags.
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);

	// May require an extra clock cycle.
	return 1;
}

u8 CPU::INC()
{
	fetch();

	// Perform increment on memory location.
	resBuf = fetched + 1;
	write(addrAbs, resBuf & 0x00FF);

	// Set flags.
	setFlag(Z, (resBuf & 0x00FF) == 0x0000);
	setFlag(N, resBuf & 0x0080);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::INX()
{
	// Perform increment on X register.
	x++;

	// Set flags.
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::INY()
{
	// Perform increment on Y register.
	y++;

	// Set flags.
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::JMP()
{
	// Perform jump.
	pc = addrAbs;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::JSR()
{
	pc--;

	// Push PC to the stack.
	write(0x0100 + sp, (pc >> 8) & 0x00FF);
	sp--;
	write(0x0100 + sp, pc & 0x00FF);
	sp--;

	// Perform subroutine jump.
	pc = addrAbs;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::LDA()
{
	fetch();

	// Perform load into the accumulator.
	a = fetched;

	// Set flags.
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);

	// May require an extra clock cycle.
	return 1;
}

u8 CPU::LDX()
{
	fetch();

	// Perform load into X register.
	x = fetched;

	// Set flags.
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);

	// May require an extra clock cycle.
	return 1;
}

u8 CPU::LDY()
{
	fetch();

	//  Perform load into Y register.
	y = fetched;

	// Set flags.
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);

	// May require an extra clock cycle.
	return 1;
}

u8 CPU::LSR()
{
	fetch();

	// Perform logical shift right.
	resBuf = fetched >> 1;

	// Set flags.
	setFlag(C, fetched & 0x0001);
	setFlag(Z, (resBuf & 0x00FF) == 0x0000);
	setFlag(N, resBuf & 0x0080);

	// Write result to the appropriate location.
	if (instructions[curOpcode].addrmode == &CPU::IMP)
		a = resBuf & 0x00FF;
	else
		write(addrAbs, resBuf & 0x00FF);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::NOP()
{
	// May require an extra clock cycle.
	return (curOpcode == 0xFC) ? 1 : 0;
}

u8 CPU::ORA()
{
	fetch();

	// Perform logical OR.
	a = a | fetched;

	// Set flags.
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);

	// May require an extra clock cycle.
	return 1;
}

u8 CPU::PHA()
{
	// Perform accumulator push to the stack.
	write(0x0100 + sp, a);
	sp--;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::PHP()
{
	// Set Break and Unused flags before push.
	setFlag(B, 1);
	setFlag(U, 1);

	// Perform status register push to stack.
	write(0x0100 + sp, status | B | U);
	sp--;
	setFlag(B, 0);
	setFlag(U, 0);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::PLA()
{
	// Pop accumulator from the stack.
	sp++;
	a = read(0x0100 + sp);

	// Set flags.
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::PLP()
{
	// Pop the status register from the stack.
	sp++;
	status = read(0x0100 + sp);

	// Always set the Unused flag.
	setFlag(U, 1);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::ROL()
{
	fetch();

	// Perform Rotate Left Through Carry.
	resBuf = (u16)(fetched << 1) | getFlag(C);

	// Set flags.
	setFlag(C, resBuf & 0xFF00);
	setFlag(Z, (resBuf & 0x00FF) == 0x0000);
	setFlag(N, resBuf & 0x0080);

	// Write result to the appropriate location.
	if (instructions[curOpcode].addrmode == &CPU::IMP)
		a = resBuf & 0x00FF;
	else
		write(addrAbs, resBuf & 0x00FF);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::ROR()
{
	fetch();

	// Perform Rotate Right Through Carry.
	resBuf = (u16)(getFlag(C) << 7) | (fetched >> 1);

	// Set flags.
	setFlag(C, fetched & 0x01);
	setFlag(Z, (resBuf & 0x00FF) == 0x00);
	setFlag(N, resBuf & 0x0080);

	// Write result to the appropriate location.
	if (instructions[curOpcode].addrmode == &CPU::IMP)
		a = resBuf & 0x00FF;
	else
		write(addrAbs, resBuf & 0x00FF);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::RTI()
{
	// Perform return from interrupt.

	// Pop status register from stack.
	sp++;
	status = read(0x0100 + sp);
	setFlag(B, 0);
	setFlag(U, 0);

	// Pop PC from the stack.
	pc = ((u16)read(0x0100 + sp + 2) << 8) | (u16)read(0x0100 + sp + 1);
	sp += 2;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::RTS()
{
	// Perform return from subroutine.

	// Pop PC from the stack.
	pc = ((u16)read(0x0100 + sp + 2) << 8) | (u16)read(0x0100 + sp + 1);
	sp += 2;

	pc++;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::SBC()
{
	fetch();

	// 16-bit add space to detect carry.

	// Invert the bottom 8 bits to make the number a positive u16,
	// turning the subtraction into an addition.
	u16 value = ((u16)fetched) ^ 0x00FF;

	resBuf = (u16)a + value + (u16)getFlag(C);

	// Set flags.
	setFlag(C, resBuf & 0xFF00);
	setFlag(Z, ((resBuf & 0x00FF) == 0));
	setFlag(V, (resBuf ^ value) & (resBuf ^ (u16)a) & 0x0080);
	setFlag(N, resBuf & 0x0080);

	// Store result in the accumulator.
	a = resBuf & 0x00FF;

	// May require an addition clock cycle.
	return 1;
}

u8 CPU::SEC()
{
	// Perform Carry Flag Set.
	setFlag(C, true);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::SED()
{
	// Perform Decimal Flag Set.
	setFlag(D, true);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::SEI()
{
	// Perform Interrupt Disable Flag set.
	setFlag(I, true);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::STA()
{
	// Store accumulator contents in memory.
	write(addrAbs, a);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::STX()
{
	// Store X register in memory.
	write(addrAbs, x);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::STY()
{
	// Store Y register in memory.
	write(addrAbs, y);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::TAX()
{
	// Transfer accumulator to X register.
	x = a;

	// Set flags.
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::TAY()
{
	// Transfer accumulator to Y register.
	y = a;

	// Set flags.
	setFlag(Z, y == 0x00);
	setFlag(N, y & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::TSX()
{
	// Transfer SP to the X register.
	x = sp;

	// Set flags.
	setFlag(Z, x == 0x00);
	setFlag(N, x & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::TXA()
{
	// Transfer X register to the accumulator.
	a = x;

	// Set flags.
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::TXS()
{
	// Transfer X register to the stack pointer.
	sp = x;

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::TYA()
{
	// Transfer Y register to the accumulator.
	a = y;

	// Set flags.
	setFlag(Z, a == 0x00);
	setFlag(N, a & 0x80);

	// Won't require an extra clock cycle.
	return 0;
}

u8 CPU::XXX()
{
	// Capture undefined opcode and do nothing.

	// Won't require an extra clock cycle.
	return 0;
}

//----------------------//
// Instruction Helpers	//
//----------------------//

u8 CPU::conditionalBranch(bool condition)
{
	// Only branch if the condition is true.
	if (condition)
	{
		// Requires an extra cycle
		// independent of the addressing mode.
		instrCycles++;

		// Perform branch.
		addrAbs = pc + addrRel;

		// Requires another extra cycle if the
		// branch changes the page of the PC.
		if ((addrAbs & 0xFF00) != (pc & 0xFF00))
			instrCycles++;

		pc = addrAbs;
	}

	// Won't require an extra cycle.
	return 0;
}

//--------------//
//	SaveState	//
//--------------//

void CPU::writeSaveStateData(std::ofstream& state)
{
    // 7 bytes.
    state.write((char*)&a, sizeof(u8));
    state.write((char*)&x, sizeof(u8));
    state.write((char*)&y, sizeof(u8));
    state.write((char*)&sp, sizeof(u8));
    state.write((char*)&pc, sizeof(u16));
    state.write((char*)&status, sizeof(u8));
    // 9 bytes.
    state.write((char*)&curOpcode, sizeof(u8));
    state.write((char*)&addrAbs, sizeof(u16));
    state.write((char*)&addrRel, sizeof(u16));
    state.write((char*)&instrCycles, sizeof(u8));
    state.write((char*)&fetched, sizeof(u8));
    state.write((char*)&resBuf, sizeof(u16));
}

void CPU::loadSaveStateData(std::ifstream& state)
{
    // 7 bytes.
    state.read((char*)&a, sizeof(u8));
    state.read((char*)&x, sizeof(u8));
    state.read((char*)&y, sizeof(u8));
    state.read((char*)&sp, sizeof(u8));
    state.read((char*)&pc, sizeof(u16));
    state.read((char*)&status, sizeof(u8));
    // 9 bytes.
    state.read((char*)&curOpcode, sizeof(u8));
    state.read((char*)&addrAbs, sizeof(u16));
    state.read((char*)&addrRel, sizeof(u16));
    state.read((char*)&instrCycles, sizeof(u8));
    state.read((char*)&fetched, sizeof(u8));
    state.read((char*)&resBuf, sizeof(u16));
}
