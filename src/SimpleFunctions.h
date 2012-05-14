#ifndef SimpleFunctions_H
#define SimpleFunctions_H

/**
 @function parse
 @param string with instruction
 @return offset of the second word (need to know to work with addresses) 
*/
int parse(const char* string);

/**
 @function get_register_name
*/
void get_register_name(char* & string, const char*  instruction);

/**
 @function compare
 Used to compare two commands. Addresses in static and dynamic analise may be different. Because of this we need to compare commands before any address will be presented.
 Function checks if first instruction begins with the second instruction.  
 @param first instruction
 @param second instruction
 @return true if equal
*/ 
bool compare(const char*, const char*);
/**
 @function condition_command
 Checks if command is condition and if second param !=NULL then compares them
 @param first instruction
 @param second instruction (may be NULL)
 @return 1 if both instructions are condition commands or first instruction is condition command and second is NULL, 0 if first instruction is not condition command, -1 if first is condition command and  second is not
*/ 
int condition_command(const char* command, const char* instruction=NULL);
int GetNumber2 (const char* string);
/**
 @function converts string to decimal number.  
*/ 
unsigned int GetNumber (const char* string);




#endif
