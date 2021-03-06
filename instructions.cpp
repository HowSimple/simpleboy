#include "CPU.h"
#include <stdexcept>
#include <iostream>
void CPU::update_halfc(u8 target, u8 source)
{
	reg.f.halfc = ((((target & 0xf) + (source & 0xf)) & 0x10) == 0x10);
}
void CPU::update_halfc(u16 target, u16 source)
{
	u8 target_high = target >> 8;
	u8 source_high = source >> 8;
	update_carry(target_high, source_high);
	//reg.f.halfc = ((((target & 0xff) + (source & 0xff)) & 0x80) == 0x80);
	//	reg.f.halfc = ((((target >> 8 & 0xf) + (source >> 8 & 0xf)) & 0x10) == 0x10);
}
void CPU::update_carry(u8 target, u8 source)
{
	reg.f.carry = ((((target & 0xff) + (source & 0xff)) & 0x80) == 0x80);
}
void CPU::update_carry(u16 target, u16 source)
{
	u8 target_high = target >> 8;
	u8 source_high = source >> 8;
	update_carry(target_high, source_high);
}

void CPU::update_zero(u8 target)
{
	reg.f.zero = (target == 0);
}
void CPU::update_zero(u16 target)
{
	reg.f.zero = (target == 0);
}

// load instrucitons
void CPU::op_ld(u8& target, u8 source) // load source to target 
{
	target = source; 
	
}

void CPU::op_ld(u16& target, u16 source) // load source to target 
{
	target = source; 
}

// increment/decrement instructions

void CPU::op_inc(u8& target) // increment target
{
	target++;
	update_halfc(target-1, target);
	update_zero(target);
	reg.f.subt = 0;
}

void CPU::op_inc(u16& target) // increment target
{
	target++;
}
void CPU::op_dec(u8& target) // decrement target 
{
	target--;
	update_halfc(target+1, target);
	update_zero(target);
	reg.f.subt = 0;
	
}
void CPU::op_dec(u16& target) // decrement target 
{
	target--;
}

void CPU::op_swap(u8& target) // swap upper/lower 4bits in target
{
	target = ((target & 0x0F) << 4 | (target & 0xF0) >> 4);
	
	reg.f.halfc = 0;
	reg.f.subt = 0;
	update_zero(target);
}

// rotate instructions
void CPU::op_rra()
{
	bool temp = reg.f.carry;
	reg.f.carry = (reg.a & (1 << 7)) >> 7;
	reg.a = ((reg.a << 1) | temp << 7);
	reg.f.halfc = 0;
	reg.f.subt = 0;
	reg.f.zero = 0;

}
void CPU::op_rrc(u8& target) // right-rotate reg A and carry flag by 1 bit
{
	bool temp = reg.f.carry;
	reg.f.carry = (target & (1 << 7)) >> 7;
	target = ( (target << 1) | reg.f.carry << 7) ;
	reg.f.zero = 0;
	reg.f.halfc = 0;
	//TODO: update_carry()

}
void CPU::op_rrca() // right-rotate reg A and carry flag by 1 bit
{
	bool temp = reg.f.carry;
	reg.f.carry = (reg.a & (1 << 7)) >> 7;
	reg.a = ((reg.a << 1) | reg.f.carry << 7);
	reg.f.halfc = 0;
	reg.f.subt = 0;
	reg.f.zero = 0;
}

void CPU::op_rr(u8& target)	// rotate right. 
{
	bool temp = reg.f.carry;
	reg.f.carry = target & 1;
	
	target = ((target >> 1) | temp << 7);
	reg.f.halfc = 0;
	reg.f.subt = 0;
	update_zero(target);
}
void CPU::op_rlca() // left-rotate reg A and carry flag by 1 bit
{
	bool temp = reg.f.carry;
	reg.f.carry = (reg.a >> 7)& 1;
	reg.f.halfc = 0;
	reg.f.subt = 0;
	reg.f.zero = 0;
	reg.a = reg.a >> 1;
}

void CPU::op_rlc(u8& target)	// rotate left. bit 7 to bit 0. bit 7 to carry.
{
	
	reg.f.carry = (target & (1 << 7)) >> 7;
	target = ((target << 1) | reg.f.carry << 7);
	update_zero(target);
	reg.f.halfc = 0;
	reg.f.subt = 0;
}
void CPU::op_rl(u8& target)	// rotate left. carry to bit 0. bit 7 to carry.
{
	bool temp = reg.f.carry;
	reg.f.carry = (target & (1 << 7)) >> 7;

	target = ((target << 1) | temp << 7);
	update_zero(target);
	reg.f.halfc = 0;
	reg.f.subt = 0;
}
// shift instructions
void CPU::op_sla(u8& target)	// shxift left. bit 7 to carry flag. bit 0 set to 0.
{
	reg.f.carry = ((target >> 7) & 1);
	target = (target << 1);
	
	reg.f.halfc = 0;
	reg.f.subt = 0;
	update_zero(target);
}
void CPU::op_sra(u8& target)	// shift right. bit 0 to carry flag. 
{// 	
	reg.f.carry = ((target >> 0) & 1);
	target = target >> 1;
	reg.f.halfc = 0;
	reg.f.subt = 0;
	update_zero(target);
}
void CPU::op_srl(u8& value)	// shift right. bit 0 to carry flag. bit 7 set to 0. 
{// TODO: needs implementation
	reg.f.carry = 0;
	reg.f.halfc = 0;
	reg.f.subt = 0;
}


// arithmetic instructions
void CPU::op_add(u8& target, u8 source) // adds value at source to target 
{
	//u8 temp
	reg.set_sub(0);
	update_carry(target, source);
	update_halfc(target, source);
	target += source;
	update_zero(target);
}

void CPU::op_add(u16& target, u16 source) // adds value at source to target 
{

	reg.set_sub(0);
	//halfcarry when carry from bit 11 to bit 12
	update_carry(target, source);
	update_halfc(target, source);
	target += source;
}

void CPU::op_sub(u8& target, u8 source) // adds value at source to target 
{
	reg.set_sub(1);
	update_carry(target, source);
	update_halfc(target, source);
	target -= source;
	update_zero(target);
}
void CPU::op_sbc(u8& target, u8 source) // subtract with carry
{
	op_sub(target, source - reg.f.carry);
}	
void CPU::op_adc(u8& target, u8 source) // adds value at source and carry to target. 
{
	op_add(target, source + reg.f.carry);
}

// logical instructions
void CPU::op_xor(u8 target) // bitwise XOR on A and target, store in A 
{
	reg.a = reg.a ^ target;
	update_zero(reg.a);
	reg.f.subt = 0;
	reg.f.halfc = 0;
	reg.f.carry = 0;
}

void CPU::op_or(u8 target) // bitwise OR on A and target, store in A 
{
	reg.a = reg.a | target;
	update_zero(reg.a);
	reg.f.subt = 0;
	reg.f.halfc = 0;
	reg.f.carry = 0;
}

void CPU::op_and(u8 target) // bitwise AND on A and target, store in A 
{
	reg.a = reg.a & target;
	update_zero(reg.a);
	reg.f.subt = 0;
	reg.f.halfc = 1;
	reg.f.carry = 0;
}

// flag instructions 
void CPU::op_ccf() // compliment carry flag
{
	reg.f.carry = !reg.f.carry;
	reg.f.halfc = 0;
	reg.f.subt = 0;
}
void CPU::op_scf() // set carry flag to true
{
	reg.f.carry = true;
	reg.f.halfc = 0;
	reg.f.subt = 0;
}

// jump instructions
void CPU::op_jp(u16 address) // jump to immediate address
{// 
	reg.pc = address;
}
void CPU::op_jpc(bool condition) // conditional jump
{
	u16 address = get_word();
	if (condition)
		op_jp(address);
	else
		cycles -= 4;
}

void CPU::op_jr(s8 address)
{
	reg.pc += address;
	
}
void CPU::op_jrc(bool condition)
{
	s8 address = get_signed_byte();
	if (condition)
		op_jr(address);
	else
		cycles -= 4;
}


// call instructions
void CPU::op_rst(u8 address)	// 'restart'. push current address to stack, jump to address
{
	push_16b(reg.pc);
	
	reg.pc = address;
	
}

void CPU::op_call(bool condition) // conditionally call address
{
	if (condition)
		op_call();
	else
	{	// TODO: cleanup. 
		cycles -= 4;
		get_word();
	}
}
void CPU::op_call() // call address
{	
	push_16b(reg.pc);
	op_jp(get_word());


}

void CPU::op_ret(bool condition) // conditionally return
{
	if (condition)
		op_ret();
	else cycles -= 4;
}
void CPU::op_ret()
{
	op_jp(pop_16b());
}
// bit instructions

void CPU::op_bit(u8 bit, u8& target) // test bit 
{
	bit = (target >> bit) & 1;
	update_zero(target);
	reg.f.subt = 0;
	reg.f.halfc = 0;
}
void CPU::op_set(u8 bit, u8& target) // set bit
{
	target |= 1 << bit;
}
void CPU::op_res(u8 bit, u8& target) // reset bit
{
	target &= ~(1 << bit);
}



// misc instructions


void CPU::op_nop() // 'no-op'
{
	
}
void CPU::op_undefined() // undefined instruction
{
	throw std::out_of_range("illegal CPU instruction");
}
void CPU::op_halt() // stop CPU until interrupt
{// TODO: needs implementation
	is_halted = 1;

}
void CPU::op_stop() // stop CPU and video, wait until next button press
{// TODO: needs implementation
	// disable LCD
}

void CPU::op_cpl(u8 target) //  set target to complement of target
{
	target = ~target;
	reg.f.subt = 1;
	reg.f.halfc = 1;
}
void CPU::op_cp(u8 value)
{
	u8 temp = reg.a - value;
	reg.set_sub(1);
	update_carry(temp, reg.a);
	update_halfc(temp, reg.a);
	update_zero(temp);
}

// interrupt instructions
void CPU::op_di() // disable interrupts after next instruction
{	
	if (!is_halted)
		execute_opcode(get_byte());
	interrupts_enabled = false;
}
void CPU::op_ei() // enable interrupts after next instruction
{
	if (!is_halted)
		execute_opcode(get_byte());
	interrupts_enabled = true;
}

void CPU::op_daa() // 
{// TODO: needs implementation
	reg.f.halfc = 0;
}
