#ifndef Instruction_Tree_H
#define Instruction_Tree_H

#include "InstructionList.h"
#include "Instruction_Leaf.h" 
#include "SimpleFunctions.h"

class PolyUnpack;

class Instruction_Tree{
	Instruction_Leaf* root;
public:
	Instruction_Tree(){root=NULL;};
	void Add(InstructionList* &, InstructionList* &, InstructionList* &);
	void Add_Leaf(Instruction_Leaf* &, int , InstructionList* &, InstructionList* &);
	int Obhod(const char*, PolyUnpack &);
	void Obhod(Instruction_Leaf* &, const char*, InstructionList* &,PolyUnpack &, int &, int &, int &);
	void Obhod2(Instruction_Leaf* &);
	int Check(Instruction_Leaf* &, InstructionList* &, InstructionList* &, InstructionList* &, InstructionList* &, bool & flag);
	bool Is_Empty();
	void Destructor(Instruction_Leaf* &);
	~Instruction_Tree();
	void CopyCommand(Instruction_Leaf* &, const char* str);
};

#endif
