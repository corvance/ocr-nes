//------------------------------------------------------------------------------//
//                                                                              //
//  OCR-NES - An NES Emulator written for the OCR A-Level                       //
//  Computer Science Programming Project.                                       //
//                                                                              //
//  Copyright (C) 2021 - 2022 Conaer Macpherson                                 //
//                                                                              //
//------------------------------------------------------------------------------//

/**
 * @file cpu.h
 * @author Conaer Macpherson (Candidate No. 6189)
 * @brief The NES 6502-based CPU.
 * @version 0.1
 * @date 2022-03-22
 *
 * @copyright Copyright (c) 2021 - 2022
 *
 */

#pragma once

// Project headers.
#include "common.h"

// Language headers.
#include <vector>

// Forward declare the Bus class to avoid circular inclusion.
class Bus;

class CPU
{
public:
	CPU();

	//--------------//
	// Registers	//
	//--------------//

	u8 a = 0x00;
	u8 x = 0x00;
	u8 y = 0x00;
	u8 sp = 0x00;
	u16 pc = 0x0000;
	u8 status = 0x00;

	/**
	 * @brief The 8 flags in the status register, enumerated for easy access.
	 */
	enum Flag
	{
		C = (1 << 0),	// Carry flag.
		Z = (1 << 1),	// Zero flag.
		I = (1 << 2),	// Interrupt disable flag.
		D = (1 << 3),	// Decimal mode (unused).
		B = (1 << 4),	// Break flag.
		U = (1 << 5),	// Unused flag (although sometimes set/unset by hardware).
		V = (1 << 6),	// Overflow flag.
		N = (1 << 7),	// Negative flag.
	};

	//--------------//
	// Execution	//
	//--------------//

	/**
     * @brief Resets the CPU to a known state.
     */
	void reset();

	/**
     * @brief Services an interrupt request.
     */
	void irq();

	/**
     * @brief Servies a Non-Maskable (can't disable) interrupt.
     */
	void nmi();

    /**
     * @brief Performs one clock-cycle of emulation.
     */
	void clockCycle();

	//----------------------//
	// Interconnect Linkage	//
	//----------------------//

	void linkInterconnect(Bus *b) { bus = b; }


private:

	//------------------//
	// Flag Operations	//
	//------------------//

    /**
     * @brief Get the boolean value of a flag in the status register.
     *
     * @param flag The flag to get the value of.
     * @return u8 1 if set, 0 if unset.
     */
	u8 getFlag(Flag f);

	/**
     * @brief Set a flag in the status register to on (1/true).
     *
     * @param flag The flag to set.
     */
	void setFlag(Flag f, bool v);

	//-------------------------//
    // Emulation Variables     //
    //-------------------------//

	// Opcode of the instruction currently being executed.
	u8 curOpcode = 0x00;
	// Absolute address.
	u16 addrAbs = 0x0000;
	// Absolute address after a branching instruction.
	u16 addrRel = 0x00;
	// Number of cycles the current instruction has left.
	u8 instrCycles = 0;
	// The fetched data for an instruction, normally ALU input or an address.
	u8 fetched = 0x00;
	// Temporary instruction result variable to avoid creating a new
	// u16 thousands of times per second.
	u16 resBuf = 0x0000;

	//--------------------------------//
	// Interconnect Linkage	(Private) //
	//--------------------------------//

	// Pointer to the interconnect bus.
	Bus *bus = nullptr;

	/**
     * @brief Reads a byte from the bus.
     *
     * @param addr The memory address to read from.
     * @return u8 The read byte/
     */
	u8 read(u16 a);

	/**
     * @brief Writes data to the bus.
     *
     * @param addr The address to write to.
     * @param data The data to write.
     */
	void write(u16 a, u8 d);

    /**
     * @brief Fetches the data an instruction needs based on its addressing mode.
     *
     * @return u8 The fetched data.
     */
	u8 fetch();

	//----------------------//
    // Addressing Modes     //
    //----------------------//

    //--------------------------------------------------------------------------//
    // The 6502 uses different addressing modes to access data in memory.       //
    // Some are direct, and some are indirect (i.e, reading an address then     //
    // reading data from the read address).                                     //
    // Each opcode describes the addressing mode its instruction uses.          //
    // Addressing precedes instruction execution as the addressing mode         //
    // changes the length of the instruction.                                   //
    //--------------------------------------------------------------------------//

	u8 IMP(); u8 IMM(); u8 ZP0();
	u8 ZPX(); u8 ZPY(); u8 REL();
	u8 ABS(); u8 ABX(); u8 ABY();
	u8 IND(); u8 IZX(); u8 IZY();

    //------------------//
    // Instructions     //
    //------------------//

    /**
     * @brief Represents 1 instruction in the 6502.
     */
    struct Instruction
    {
        // The instruction's implementation function.
        u8 (CPU::*impl)(void) = nullptr;
        // The instruction's addressing mode.
        u8 (CPU::*addrmode)(void) = nullptr;
        // The base number of cycles the instruction takes.
        u8 cycles = 0;
    };

	std::vector<Instruction> instructions;

	u8 ADC(); u8 AND(); u8 ASL(); u8 BCC();
	u8 BCS(); u8 BEQ(); u8 BIT(); u8 BMI();
	u8 BNE(); u8 BPL(); u8 BRK(); u8 BVC();
	u8 BVS(); u8 CLC(); u8 CLD(); u8 CLI();
	u8 CLV(); u8 CMP(); u8 CPX(); u8 CPY();
	u8 DEC(); u8 DEX(); u8 DEY(); u8 EOR();
	u8 INC(); u8 INX(); u8 INY(); u8 JMP();
	u8 JSR(); u8 LDA(); u8 LDX(); u8 LDY();
	u8 LSR(); u8 NOP(); u8 ORA(); u8 PHA();
	u8 PHP(); u8 PLA(); u8 PLP(); u8 ROL();
	u8 ROR(); u8 RTI(); u8 RTS(); u8 SBC();
	u8 SEC(); u8 SED(); u8 SEI(); u8 STA();
	u8 STX(); u8 STY(); u8 TAX(); u8 TAY();
	u8 TSX(); u8 TXA(); u8 TXS(); u8 TYA();

	// Function to capture the undefined opcodes in the NES' 6502 ISA.
	u8 XXX();

	//----------------------//
	// Instruction Helpers	//
	//----------------------//

	/**
     * @brief All conditional branches are the same except for the flag used, so saves code.
     */
	u8 conditionalBranch(bool condition);

public:

    //--------------//
    //	SaveState	//
    //--------------//

    /**
     * @brief Writes save state data for the CPU to an std::ofstream.
     *
     * @param state The save state std::ofstream.
     */
    void writeSaveStateData(std::ofstream& state);

    /**
     * @brief Loads save state data for the CPU from an std::ifstream.
     *
     * @param state The save state std::ofstream.
     */
    void loadSaveStateData(std::ifstream& state);
};
