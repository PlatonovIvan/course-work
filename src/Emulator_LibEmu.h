#ifndef Emulator_LibEmu_H
#define Emulator_LibEmu_H


enum Register {
	EIP,EBP,ESP,ESI,EDI,
	IP,BP,SP,SI,DI,
	EAX,EBX,ECX,EDX,
	AX,BX,CX,DX,
	AH,BH,CH,DH,
	AL,BL,CL,DL,
	HASFPU
};

/**
 @class emulates execution per step.
*/

class Emulator_LibEmu{
	struct emu *e; 
	struct emu_cpu *cpu;
	struct emu_memory *mem;
	unsigned int im_base; ///image base (from what address we need to load file for emulation)
public:
/**
 Constructor
*/ 
	Emulator_LibEmu();
/**
 Destructor
*/
	~Emulator_LibEmu();
/**
 Loads file in emulator memory, sets all registers in 0 and sets eip to the entry point
 @param buf buffer
 @param len buffer size
 @param entry_point entry point
 @param base image base
*/ 	
	void begin(char* & buf, const unsigned int len, const unsigned int entry_point, const unsigned int base);
/**
 Sets eip to the nesessary address
 @param pos address
*/ 	
	void jump(const unsigned int pos);
/**
 Emulates execution of one instruction. Calls two functions bool emu_cpu_parse(cpu) and bool emu_cpu_step(cpu).
 @return false if one of this functions return false. In another case returns true.
*/ 
	bool step();
/**
 Reads size bytes into the buffer from emulator memory
 @param buf buffer where to read
 @param size size of buffer 
*/ 
	void get_command(char *buf, const unsigned int size) const;
/**
 Helps to work with buffer after emulation
 @param buf buffer
 @param size size of buffer
*/ 	
	void read_block(char* buf, const unsigned int size) const;
/**
 Gets value of register reg
 @param reg register
 @return value
*/ 	
	unsigned int get_register(const Register reg) const;
/**
 Gets value of register
 @param string string with register name
 @return value 
*/ 	
	unsigned int get_register(char* & string) const;
};


#endif
