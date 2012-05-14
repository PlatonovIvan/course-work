#ifndef InstructionList_H
#define InstructionList_H
/**
 @struct InstructionList
 List of disassembled instructions from Static Analise 
*/ 
struct InstructionList{
	char* instruction; /// disassembled instruction in INTEL format
	int number; ///offset of instruction from text section
	int length; ///length of instruction
	bool first; ///is true is if instruction is entry point, is false in another case 
	bool padding; ///is true is if instruction is padding, is false in another case
	InstructionList* pointer; ///pointer to the next InstructionList
};

#endif
