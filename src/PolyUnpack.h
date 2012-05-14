#ifndef PolyUnpack2_H
#define PolyUnpack2_H

#include "InstructionList.h"
#include "Instruction_Leaf.h"
#include "Emulator_LibEmu.h"
#include "PE_Reader.h"
#include "Stack.h"
#include "SimpleFunctions.h"


class PolyUnpack; 
/**
 @class In process of execution it needs to find the right way in case 
 of condition jump. So that we can compare instructions properly. For 
 this target Instruction_Tree is used. It helps to control any possible 
 variants of execution. It is a bit difficult to understand logic of 
 members of this class. If you have any problems, please contact me. 
 I will be glad to explain it.
*/
class Instruction_Tree{
	Instruction_Leaf* root; ///root of the tree. Contains some condition jump command
public:
/**
 Constructor by default
*/
	Instruction_Tree(){root=NULL;};
/**
 Add condition jump command in root with two possible instructions, which will be executed next,  as leafs. 
 @param po1 condition inctruction
 @param po2 next instruction after condition jump (left leaf)
 @param po3 another instruction (right leaf)
*/ 
	void Add(InstructionList* & po1, InstructionList* & po2, InstructionList* & po3);
/*
 Add leaf if root is not empty
 @param leaf in a tree where to add instruction
 @param first instruction (left leaf)
 @param next instruction (right leaf). Is null if leaf is not condition jump.
*/ 
//	void Add_Leaf(Instruction_Leaf* &, int , InstructionList* &, InstructionList* &);
/**
 When root is not empty, we need to compare executed instruction with 
 all leaf instructions in the tree and check amount of legal leafs. If 
 there are not any legal instructions or just one, we need to delete tree.        
 @param command executed command to compare with
 @param unpack We need to compare command with leaf and if it is legal to find another possible ways
 @return 0 if there are not any legal instructions, 1 in another case return a number>0.   
*/ 	
	int Obhod(const char* command, PolyUnpack& unpack);
/**
 The search is performed only in legal branches. To do this we compare 
 'legal' field in every node. If leaf is valid, one or two branches are 
 added to it. This function uses recursion!
 @param p1 temporary root
 @param command
 @param original pointer to InstructionList
 @param unpack
 @param kol number of valid leafs
 @param iterator
 @param result
*/ 
	void Obhod(Instruction_Leaf* & p1, const char* command,
			InstructionList* & original, PolyUnpack & unpack, 
					int & kol, int & iterator, int & result);
/**
 Print all tree
 @param p1 temporary root
*/ 	
	void Print(Instruction_Leaf* & p1);
/**
 Sometimes all valid leafs contain the same condition instructrion, as a
 root. So we have 'loop'. This function checks for this loops and deletes them. 
 This function uses recursion!
 @param p1 temporary root
 @param po1 pointer to instruction in original root
 @param po2 pointer to instruction in left leaf under original root
 @param po3 pointer to instruction in right leaf under original root 
 @param original pointer to instruction in original instruction
 @param flag is true if loop is presented
*/ 
	int Check(Instruction_Leaf* & p1, InstructionList* & po1, 
				InstructionList* & po2, InstructionList* & po3, 
				InstructionList* & original, bool & flag);
/**
 Checks if root is empty
 @return true if NULL, false if is not NULL.
*/ 	
	bool Is_Empty();
/**
 Deletes all tree and makes root NULL. Function uses recursion.
 @param p1 temporary root
*/
	void Destructor(Instruction_Leaf* & p1);
/**
 Destructor
*/ 	
	~Instruction_Tree();
	
	//void CopyCommand(Instruction_Leaf* &, const char* str);
};
/**
 @class contains the main logic of the algorithm
*/ 
class PolyUnpack{
	InstructionList* start;///start of the first instruction in list instruction 
	Instruction_Tree root; /// root of Struct Instruction_Tree
	Emulator_LibEmu* emulator; ///Emulator Libemu
	Stack return_addresses; /// Stack
	unsigned int buf_size; ///size of buffer
	unsigned int base; /// base 
	unsigned int offset; /// physical offset of code section
	unsigned int code_size; /// size of code section
	int step_kol; /// =3500 (recomended) numbers of steps before emulation will be halted
	int invalid_instr_kol; /// number of invalid instructions in file
public:
/**
 Constructor
*/ 
	PolyUnpack();
/**
 Destructor
*/ 	
	~PolyUnpack();
/**
 Is used in Static Analise to add disassembled instructions in list
 @param p1 last instruction in list
 @param string disassembled instruction in INTEL format
 @param number offset of instruction from code section
 @param length length of instruction
 @param flag true if instruction is entry point, false in other case
*/ 
	void AddInstruction(InstructionList* &p1, const char* string, 
						const int number, const int length, 
						const bool flag);
/**
 All sections in PE format cotain padding opcodes 00 or add eax,al. We 
 don't need to execute them. When the eip points to this instructions the
 execution halts.
*/ 	
	void FindPadding() const;
/**
 Finds instruction, which is entry point.
 @param p pointer to the entry point instruction
 @return true if it is found, false in another case 
*/ 
	bool FindFirstInstruction(InstructionList* & p) const;
/**
 Finds next instruction.
 @param p pointer to the instruction
 @param number offset of this instruction
 @return true if it is found, false in another case
*/ 
	bool FindInstruction(InstructionList* & p, const int number) const;
/**
 @param number offset of this instruction
 @return true if it is valid
*/ 	
	bool is_valid_number(int number) const;
/**
 Generates static code view and list of disassembled instructions
 @param FileName file name to be disassembled
 @param key is 1 to see static disassembling, =0 in other case 
*/ 
	void StaticAnalise(const char* FileName,  const int key);
/**
 Generates Dynamic analise. Prints number of unvalid instructions or message "File is valid". 
 Emulator ignores calls and jumps outside the program. 
 
 @param FileName file name to be executed
 @param key is 1 to see dynamic disassembling, =0 in other case
*/ 
	void DynamicAnalise(const char* FileName, const int key);
/**
 Compares command that was executed with its static view.
 @param p static view of supposed instruction
 @param command command that was just executed
 @param flag flag of result
 @param old_eip eip pointer before execution
 @return <ul>
			<li> 1 if commands are simple and equal
			<li> -1 if commands are not equal 
			<li> 0 if commands are condition and equal 
			<li> 2 if when instruction call[] or jmp[] was found
			<li> 3 when instruction call, ret, retn was found
			<li> 4 when stack is empty
			<li> 5 when invalid address in case of jump reg or call reg
		 </ul>
*/  
	int Compare(InstructionList* &p, const char* command, int flag, const unsigned int old_eip); 
												   //1 is true and simple instruction
												   //-1 is false
												   //0 is unknown in case of conditional jump jz/jnz/...
												   //2 when instruction call[], jmp[] was found. Need 
												   //to do emulator.jump()because in other case EIP could be 0
												   //3 when instruction call, ret, retn was found
												   //4 when stack is empty
												   //5 invalid address in register case
/**
 Finds next instruction
 @param p first instruction
 @param p2 second instruction
 @param command executed command
 @param iterator we need to push and pop addresses  from stack just once
 @return <ul>
			<li> 1 if commands are simple and equal
			<li> -1 if commands are not equal 
			<li> 0 if commands are condition and equal 
			<li> 2 if when instruction call[] or jmp[] was found
			<li> 3 when instruction call, ret, retn was found
			<li> 4 when stack is empty
			<li> 5 when invalid address in case of jump reg or call reg
			<li> 6 int XX was found. We need to ignre next instruction until sys call would not end  
		 </ul>
*/ 
	int FindNextInstruction(InstructionList* &p, InstructionList* & p2, const char* command, int & iterator);
/**
 Helps to work with buffer after emulation
 @param buf buffer
*/ 	
	void BinaryDump();
};
 
#endif
