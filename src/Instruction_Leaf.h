#ifndef Instruction_Leaf_H
#define Instruction_Leaf_H

#include "InstructionList.h"
/**
 @struct Used as a node/leaf in class Instruction_Tree. 
*/ 
struct Instruction_Leaf{
	InstructionList* pointer; ///pointer to struct InstructionList. So that we can use member char* instruction in InstructionList.
	Instruction_Leaf* left; ///pointer to the left node/leaf in Instruction_Tree. If it is leaf then left=NULL. 
	Instruction_Leaf* right; ///pointer to the right node/leaf in Instruction_Tree. If it is leaf then right=NULL.
	int valid; /// =1 if node valid / =-1 if node is not valid / =0  if it is leaf and we don't know yet
			
};


#endif
