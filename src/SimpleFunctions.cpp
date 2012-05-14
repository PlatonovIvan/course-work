#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SimpleFunctions.h"

using namespace std;

int parse(const char* string){
	int i=0;
	while(string[i]!=' ')
		i++;
	while(string[i]==' ')
		i++;
	return i;
} 

void get_register_name(char* & string, const char* instruction){
	int n=4;
	int i=0;
	string=new char[strlen(instruction)];
	while(instruction[i]!='\0'){
		string[i]=instruction[i];
		i++;	
	}
	string[i]='\0';
}

bool compare(const char* string1, const char* string2){
	int i=0;
	while((string1[i]!='\0')&&(string2[i]!='\0')){
		if (string1[i]!=string2[i])
			return false;
		i++;
	}
	return true;
}

int condition_command(const char* command, const char*  instruction){
	const int n=27;
	const char* cond_com[n]={"jz", "jnz", "js", "jns", "jmp", "ja", 
								"jna", "jo", "jno", "jnc", "jc", "jl", 
								"jnl", "js", "jns", "jecxz","jp", "jpo",
							    "jg", "jng", "jmpf", "loopn", "loope", 
							    "loopne", "loopz", "loopnz", "loop"};
	int flag=0;
	for (int i=0; i<n; i++){
		if (compare(command, cond_com[i])){
			flag=1;
			break;
		}
	}
	
	if ((instruction==NULL)||(flag==0)){
		return flag;
	}
	
	flag=-1;
	for (int i=0; i<n; i++){
		if ((compare(command, cond_com[i]))&&(compare(instruction, cond_com[i]))){
			flag=1;
			break;
		}
	}
	return flag;			
}

int GetNumber2 (const char* string){
	int koef=16;
	int number=0;
	int i=0;
	while(string[i]!='\0'){
		if ((string[i]>='0')&&(string[i]<='9')){
			number=number*koef+(string[i]-'0');
		}
		else{
			if ((string[i]>='a')&&(string[i]<='f'))
				number=number*koef+(string[i]-'a'+10);
			else{
				//printf("Invalid number\n");
				number=0;
				break;
			}
		}
		i++;
	}
	return number;
}

/*
unsigned int GetNumber (const char* string){
	unsigned int segment=0;
	unsigned int offset=0;
	int koef=16;
	unsigned int number=0;
	int i=0;
	while(string[i]!='\0'){
		if ((string[i]>='0')&&(string[i]<='9')){
			offset=offset*koef+(string[i]-'0');
		}
		else{
			if ((string[i]>='a')&&(string[i]<='f'))
				offset=offset*koef+(string[i]-'a'+10);
			else{
				if (string[i]==':'){
					i=i+3;
					segment=offset;
				}
				offset=0;
				continue;
			}
		}
		i++;
	}
	number=segment*pow(koef,4)+offset;
	return number;
}
*/

unsigned int GetNumber (const char* string){
	unsigned int segment=0;
	unsigned int offset=0;
	int koef=16;
	unsigned int number=0;
	int i=0;
	while(string[i]!='\0'){
		if ((string[i]>='0')&&(string[i]<='9')){
			offset=offset*koef+(string[i]-'0');
		}
		else{
			if ((string[i]>='a')&&(string[i]<='f'))
				offset=offset*koef+(string[i]-'a'+10);
			else{
				if (string[i]==':'){
					i=i+2;
					segment=offset;
				}
				offset=0;
				break;
			}
		}
		i++;
	}
	number=segment*pow(koef,4)+offset;
	return number;
}
