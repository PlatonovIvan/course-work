#ifndef Stack_H
#define Stack_H

/**
 @class List_of_Return_Addresses
	Stores information about return addresses
*/
struct List_of_Return_Addresses{
	unsigned int address; ///address of the next instruction after call 0xXXXX instruction
	List_of_Return_Addresses* pointer; ///pointer to the next List_of_Return_Addresses
};

/**
 @class Stack
*/

class Stack{
	List_of_Return_Addresses *start; ///first address in Stack
public:
/**
  default constructor
*/
	Stack();
/**
  Pushes next address in Stack
  @param a address
*/	
	void Push(const unsigned int a);
/**
  Pops next address from Stack
  @param a address
*/	
	void Pop(unsigned int & a);
/**
 Prints all addresses in stack 
*/	
	void Print();
/**
 default destructor
*/	
	~Stack();
};

#endif
