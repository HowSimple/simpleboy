//#include "Registers.h"
//#include "Registers.cpp"
#include "CPU.h"


void CPU::update_halfc(u8 target, u8 source)
{
	f.halfc = ((((target & 0xf) + (source & 0xf)) & 0x10) == 0x10);
}
void CPU::update_halfc(u16 target, u16 source)
{
	f.halfc = ((((target & 0xff) + (source & 0xff)) & 0x80) == 0x80);
}
void CPU::update_carry(u8 target, u8 source)
{
	f.carry = ((((target & 0xf) + (source & 0xf)) & 0x10) == 0x10);
}
void CPU::update_carry(u16 target, u16 source)
{
	f.carry = ((((target & 0xff) + (source & 0xff)) & 0x80) == 0x80);
}

void CPU::update_zero(u8 target)
{
	f.zero = (target == 0);
}
void CPU::update_zero(u16 target)
{
	f.zero = (target == 0);
}

// load instrucitons
void CPU::op_ld(u8& target, u8 source) // load source to target 
{
	target = source; 
	
}
void CPU::op_ld(u16& target, u8 source) // load source to target 
{
	target = source;
	
}
void CPU::op_ld(u8& target, u16 source) // load source to target 
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
	u8 temp = target;
	target++;
	f.zero = 0;
	f.subt = 0;
	update_halfc(target, temp);
}

void CPU::op_inc(u16& target) // increment target
{
	target++;
}
void CPU::op_dec(u8& target) // decrement target 
{
	update_halfc(target, --target);
	//target--;
	update_zero(target);
	f.subt = 0;
	
}

void CPU::op_dec(u16& target) // decrement target 
{
	target -= 1; 
}





void CPU::op_swap(u8& target) // swap upper/lower 4bits in target
{
	target = target << 4;
	f.halfc = 0;
	f.subt = 0;
	update_zero(target);
}
void CPU::op_swap(u16& target) // swap upper/lower 8bits in target
{
	target = target << 8;
	f.halfc = 0;
	f.subt = 0;
	update_zero(target);
}

// rotate instructions
void CPU::op_rra()
{
	bool temp = f.carry;
	f.carry = (a & (1 << 7)) >> 7;
	a = ((a << 1) | temp << 7);
	update_zero(a);

}
void CPU::op_rrc(u8& reg) // right-rotate reg A and carry flag by 1 bit
{
	bool temp = f.carry;
	f.carry = (a & (1 << 7)) >> 7;
	a = ( (a << 1) | f.carry << 7) ;
	f.zero = 0;
	f.halfc = 0;
	//TODO: update_carry()

}
void CPU::op_rrca() // right-rotate reg A and carry flag by 1 bit
{
	bool temp = f.carry;
	f.carry = (a & (1 << 7)) >> 7;
	a = ((a << 1) | f.carry << 7);
	f.halfc = 0;
	f.subt = 0;
	f.zero = 0;
}

void CPU::op_rr(u8& value)	// rotate right. 
{
	bool temp = f.carry;
	f.carry = (a & (1 << 7)) >> 7;

	a = ((a << 1) | temp << 7);
	f.halfc = 0;
	f.subt = 0;
	update_zero(a);
}
void CPU::op_rlca() // left-rotate reg A and carry flag by 1 bit
{
	bool temp = f.carry;
	f.carry = (1 & (a >> 7));
	//f.carry = (a & (1 << 7)) >> 7;
	a = a >> 1;
	update_zero(a);
}

void CPU::op_rlc(u8& reg)	// rotate left. bit 7 to bit 0. bit 7 to carry.
{
	
	f.carry = (reg & (1 << 7)) >> 7;
	reg = ((reg << 1) | f.carry << 7);
	update_zero(reg);
	f.halfc = 0;
	f.subt = 0;
}
void CPU::op_rl(u8& reg)	// rotate left. carry to bit 0. bit 7 to carry.
{
	bool temp = f.carry;
	f.carry = (a & (1 << 7)) >> 7;

	a = ((a << 1) | temp << 7);
	update_zero(a);
	f.halfc = 0;
	f.subt = 0;
}
// shift instructions
void CPU::op_sla(u8& value)	// shift left. bit 7 to carry flag. bit 0 set to 0.
{// TODO: needs implementation
	f.halfc = 0;
	f.subt = 0;
}
void CPU::op_sra(u8& value)	// shift right. bit 0 to carry flag. 
{// TODO: needs implementation
	f.carry = 0;
	f.halfc = 0;
	f.subt = 0;
}
void CPU::op_srl(u8& value)	// shift right. bit 0 to carry flag. bit 7 set to 0. 
{// TODO: needs implementation
	f.carry = 0;
	f.halfc = 0;
	f.subt = 0;
}


// arithmetic instructions
void CPU::op_add(u8& target, u8 source) // adds value at source to target 
{
	update_carry(target, source);
	update_halfc(target, source);
	target += source;
	update_zero(target);
}

void CPU::op_add(u16& target, u16 source) // adds value at source to target 
{
	update_carry(target, source);
	update_halfc(target, source);
	target += source;
	update_zero(target);
}

void CPU::op_sub(u8& target, u8 source) // adds value at source to target 
{
	set_sub(1);
	update_carry(target, source);
	update_halfc(target, source);
	target -= source;
	update_zero(target);
}
void CPU::op_sbc(u8& target, u8 source) // subtract with carry
{// TODO: needs implementation

}	
void CPU::op_adc(u8& target, u8 source) // adds value at source and carry to target. 
{
	set_sub(0);
	update_carry(target, source);
	update_halfc(target, source);
	target += source;
	update_zero(target);

}





// logical instructions
void CPU::op_xor(u8 target) // bitwise XOR on A and target, store in A 
{
	a = a ^ target;
	update_zero(a);
}
void CPU::op_xor(u16 target) // bitwise XOR on A and target, store in A 
{
	a = a ^ target;
	update_zero(a);
	f.subt = 0;
	f.halfc = 0;
	f.carry = 0;
}
void CPU::op_or(u8 target) // bitwise OR on A and target, store in A 
{
	a = a | target;
	update_zero(a);
	
}
void CPU::op_or(u16 target) // bitwise OR on A and target, store in A 
{
	a = a | target;
	update_zero(a);
	f.subt = 0;
	f.halfc = 0;
	f.carry = 0;
}
void CPU::op_and(u8 target) // bitwise AND on A and target, store in A 
{
	a = a & target;
	update_zero(a);
	f.subt = 0;
	f.halfc = 1;
	f.carry = 0;
}
void CPU::op_and(u16 target) // bitwise AND on A and target, store in A 
{
	a = a & target;
	update_zero(a);
	f.subt = 0;
	f.halfc = 1;
	f.carry = 0;	
}
// flag instructions 
void CPU::op_ccf() // compliment carry flag
{
	f.carry = ~f.carry;
	f.halfc = 0;
	f.subt = 0;
}
void CPU::op_scf() // set carry flag to true
{
	f.carry = true;
	f.halfc = 0;
	f.subt = 0;
}

// jump instructions
void CPU::op_jp() 
{// TODO: needs implementation
	
}
void CPU::op_jp(bool condition) // push reg to stack
{
	if (condition)
		op_jp();
	else cycles -= 4;
}
void CPU::op_jr()
{// TODO: needs implementation

}
void CPU::op_jr(bool condition)
{
	if (condition)
		op_jr();
	else cycles -= 4;
}

// stack instructions
void CPU::op_pop(u16& reg) // pop 2 bytes from stack to register
{
	reg = (ram[sp] << 8) | (ram[sp - 1]);
	sp += 2;
	
}
void CPU::op_push(u16 value) // push value to stack
{
	ram[sp] = value;
	sp -= 2;
	
}
// call instructions
void CPU::op_rst(u8 address)	// 'restart'. push current address to stack, jump to address
{
	op_call(address);
}

void CPU::op_call(bool condition) // conditionally call address
{
	if (condition)
		op_call();
	else cycles -= 4;
}
void CPU::op_call() // call address
{	// TODO: needs implementation



}

void CPU::op_ret(bool condition) // conditionally return
{
	if (condition)
		op_ret();
	else cycles -= 4;
}
void CPU::op_ret()
{// TODO: needs implementation

}
// bit instructions

void CPU::op_bit(u1 bit, u8& reg) // test bit 
{
	//u1 bit = (reg >> bit) & 1;
	update_zero(reg);
	f.subt = 0;
	f.halfc = 0;
}
void CPU::op_set(u1 bit, u8& reg) // set bit
{
	reg |= 1 << bit;
}
void CPU::op_res(u1 bit, u8& reg) // reset bit
{
	reg &= ~(1 << bit);
}



// misc instructions


void CPU::op_nop() // 'no-op'
{// TODO: needs implementation
	
}
void CPU::op_undefined() // undefined instruction
{// TODO: needs implementation

}
void CPU::op_halt() // stop CPU until interrupt
{// TODO: needs implementation

}
void CPU::op_stop() // stop CPU and video, wait until next button press
{// TODO: needs implementation

}

void CPU::op_cpl(u8 target) //  set target to complement of target
{
	target = ~target;

}
void CPU::op_cp(u16 target)
{// TODO: needs implementation


}

// interrupt instructions
void CPU::op_di() // disable interrupts after next instruction
{// TODO: needs implementation

}
void CPU::op_ei() // enable interrupts after next instruction
{// TODO: needs implementation

}

void CPU::op_daa() // enable interrupts after next instruction
{// TODO: needs implementation
	f.halfc = 0;
}
