#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PolyUnpack.h"
extern "C"{
	#include "libdasm/libdasm.h"
}
using namespace std;
void Instruction_Tree:: Add(InstructionList* & po1, InstructionList* & po2, InstructionList* & po3){
	Instruction_Leaf* p;
	if (root==NULL){
		root=new Instruction_Leaf;
		root->pointer=po1;
		root->valid=1;
		if (po2!=NULL){
			//printf("Left Leaf=%s\n", po2->instruction);
			root->left=new Instruction_Leaf;
			
			p=root->left;
			p->pointer=po2;
			p->valid=0;
			p->left=NULL;
			p->right=NULL;
		}
		else{
			//printf("Left Leaf=NULL");
			root->left=NULL;
		}
		if (po3!=NULL){
			//printf("Right Leaf=%s\n", po3->instruction);
			root->right=new Instruction_Leaf;
			p=root->right;
			p->pointer=po3;
			p->valid=0;
			p->left=NULL;
			p->right=NULL;
		}
		else{
			//printf("Right Leaf=NULL\n");
			root->right=NULL;
		}
	}
}

int Instruction_Tree::Obhod(const char* command, PolyUnpack& unpack){
	int kol=0;
	int  iterator=0;
	int result=0;
	//Print(root);
	kol=0;
	iterator=0;
	result=0;
	InstructionList* original=NULL;
	Obhod(root, command, original, unpack, kol, iterator, result);
	//printf("KOL_of_Valid=%d\n", kol);
	if (kol==0){
		Destructor(root);
		return -1;
	}
	if (kol==1){
		Destructor(root);
		return result;
	}
	if (kol>1)
		if (condition_command(command)==1){
			int i=parse(command);
			if (command[i]!='['){
				bool flag=true;
				InstructionList* po1=NULL;
				InstructionList* po2=NULL;
				InstructionList* po3=NULL;
				Check(root, po1, po2, po3, original, flag);
				if (flag==true){
					Destructor(root);
					//root=NULL;
					//InstructionList* po2=NULL;
					//InstructionList* po3=NULL; 
					int iterator=0;
					Add(po1, po2, po3);
					//Print(root);
				}
			}
		}
		return result;
}

void Instruction_Tree::Obhod(Instruction_Leaf* & p1, const char* command,
							InstructionList* & original, PolyUnpack & unpack, 
									int & kol, int & iterator, int & result){
	Instruction_Tree* p2;
	if ((p1->left!=NULL)&&(p1->right!=NULL)){
		
		if (p1->left->valid!=-1)
			Obhod(p1->left, command, original, unpack, kol, iterator, result);
		
		if (p1->right->valid!=-1)
			Obhod(p1->right, command, original, unpack, kol, iterator, result);
		
		if ((p1->left->valid==-1)&&(p1->right->valid==-1))
			p1->valid=-1;
		
		return;
	}
	if ((p1->left!=NULL)&&(p1->right==NULL)){
		
		if (p1->left->valid!=-1)
			Obhod(p1->left, command, original, unpack, kol, iterator, result);
		
		if (p1->left->valid==-1)
			p1->valid=-1;
		
		return;
	}
	if ((p1->left==NULL)&&(p1->right!=NULL)){
		
		if (p1->right->valid!=-1)
			Obhod(p1->right, command, original, unpack, kol, iterator, result);
		
		if (p1->right->valid==-1)
			p1->valid=-1;
		
		return;
	}
	if ((p1->left==NULL)&&(p1->right==NULL)){
		
		if ((p1->valid==0)){
			//printf("Next Leaf=%s\n", p1->pointer->instruction);
			//printf("Command=%s\n", command);
			InstructionList* po2=p1->pointer;
			InstructionList* po3=NULL;
			int flag;
			flag=unpack.FindNextInstruction(po2, po3, command, iterator);
			
			if (result<flag){
				result=flag;
			}
			
			if (flag==-1){
				p1->valid=-1;
				return;
			}
			
			if ((flag==1)||(flag==2)||(flag==3)){
				original=p1->pointer;
				p1->valid=1;
				p1->left=new Instruction_Leaf;
				Instruction_Leaf* p2=p1->left;
				p2->pointer=po2;
				p2->valid=0;
				p2->left=NULL;
				p2->right=NULL;
				kol++;
				return;
			}
			
			if((flag==4)||(flag==5)||(flag==6)){
				kol=1;
				return;
			}
			
			if (flag==0){
				original=p1->pointer;
				p1->valid=1;
				p1->left=new Instruction_Leaf;
				Instruction_Leaf* p2=p1->left;
				p2->pointer=po2;
				p2->valid=0;
				p2->left=NULL;
				p2->right=NULL;
				if (po3!=NULL){
					p1->right=new Instruction_Leaf;
					p2=p1->right;
					p2->pointer=po3;
					p2->valid=0;
					p2->left=NULL;
					p2->right=NULL;
				}
				kol++;
			}
		}
	}
}

void Instruction_Tree::Print(Instruction_Leaf* & p1){
	Instruction_Tree* p2;
	if ((p1->left!=NULL)&&(p1->right!=NULL)){
		//printf("(p1->left!=NULL)&&(p1->right!=NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		Print(p1->left);
		Print(p1->right);
		return;
	}
	if ((p1->left!=NULL)&&(p1->right==NULL)){
		//printf("(p1->left!=NULL)&&(p1->right==NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		Print(p1->left);
		return;
	}
	if ((p1->left==NULL)&&(p1->right!=NULL)){
		//printf("(p1->left==NULL)&&(p1->right!=NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		Print(p1->right);
		return;
	}
	if ((p1->left==NULL)&&(p1->right==NULL)){
		//printf("(p1->left==NULL)&&(p1->right==NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		return;
	}
}

int Instruction_Tree::Check(Instruction_Leaf* & p1, InstructionList* & po1, 
							InstructionList* & po2, InstructionList* & po3, 
								InstructionList* & original, bool & flag){
	if ((p1->left!=NULL)&&(p1->right!=NULL)){
		//printf("(p1->left!=NULL)&&(p1->right!=NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		if (p1->left->valid!=-1){
			int temp=Check(p1->left, po1, po2, po3, original, flag);
			if (temp==1){
				po1=p1->pointer;
				po2=p1->left->pointer;
				//printf("p1->pointer->instruction=%s\n", p1->pointer->instruction);
				//printf("command=%s\n", original->instruction);
				if (strcmp(p1->pointer->instruction, original->instruction)!=0){
					flag=false;
				}		 
			}
		}
		if (p1->right->valid!=-1){
			int temp=Check(p1->right, po1, po2, po3, original, flag);
			if (temp==1){
				po1=p1->pointer;
				po3=p1->right->pointer;
				//printf("p1->pointer->instruction=%s\n", p1->pointer->instruction);
				//printf("command=%s\n", original->instruction);
				if (strcmp(p1->pointer->instruction, original->instruction)!=0){
					flag=false;
				}		 
			}
		}
		return 0;
	}
	
	if ((p1->left!=NULL)&&(p1->right==NULL)){
		//printf("(p1->left!=NULL)&&(p1->right==NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		if (p1->left->valid!=-1){
			int temp=Check(p1->left, po1, po2, po3, original, flag);
			if (temp==1){
				po1=p1->pointer;
				po2=p1->left->pointer;
				//printf("p1->pointer->instruction=%s\n", p1->pointer->instruction);
				//printf("command=%s\n", original->instruction);
				if (strcmp(p1->pointer->instruction, original->instruction)!=0){
					flag=false;
				}		 
			}
		}
		return 0;
	}
	
	if ((p1->left==NULL)&&(p1->right!=NULL)){
		//printf("(p1->left==NULL)&&(p1->right!=NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		if (p1->right->valid!=-1){
			int temp=Check(p1->right, po1, po2, po3, original, flag);
			if (temp==1){
				po1=p1->pointer;
				po3=p1->right->pointer;
				//printf("p1->pointer->instruction=%s\n", p1->pointer->instruction);
				//printf("command=%s\n", original->instruction);
				if (strcmp(p1->pointer->instruction, original->instruction)!=0){
					flag=false;
				}		 
			}
		}
		return 0;
	}
	if ((p1->left==NULL)&&(p1->left==NULL)&&(p1->right==NULL)&&(p1->right==NULL)){
		//printf("(p1->left==NULL)&&(p1->right==NULL)\n"); 
		return 1;
	}
}

void Instruction_Tree::Destructor(Instruction_Leaf*  & p1){
	if ((p1->left!=NULL)&&(p1->right!=NULL)){
		Destructor(p1->left);
		Destructor(p1->right);
		delete p1;
		p1=NULL;
		return;
	}
	if ((p1->left!=NULL)&&(p1->right==NULL)){
		Destructor(p1->left);
		delete p1;
		p1=NULL;
		return;
	}
	if ((p1->left==NULL)&&(p1->right!=NULL)){
		Destructor(p1->right);
		delete p1;
		p1=NULL;
		return;
	}
	if ((p1->left==NULL)&&(p1->right==NULL)){
		delete p1;
		p1=NULL;
		return;
	}
}

Instruction_Tree::~Instruction_Tree(){
	if (root!=NULL)
		Destructor(root);
}

bool Instruction_Tree::Is_Empty(){
	if (root==NULL)
		return true;
	return false;
}




PolyUnpack::PolyUnpack(){
	start=NULL;
	buf_size=0;
	base=0;
	code_size=0;
	offset=0;
	step_kol=3500;
	invalid_instr_kol=0;
	emulator=new Emulator_LibEmu;
}

PolyUnpack::~PolyUnpack(){
	InstructionList* p1=start;
	InstructionList* p2;
	while(p1!=NULL){
		p2=p1->pointer;
		delete []p1->instruction;
		delete p1;
		p1=p2;
	}
	delete emulator;
}

void PolyUnpack::AddInstruction(InstructionList* &p1, const char* string, const int number, const int length, const bool flag){
	if (start==NULL){
		start=new InstructionList;
		p1=start;
	}
	else{
		p1->pointer=new InstructionList;
		p1=p1->pointer;
	}
	p1->instruction=new char[strlen(string)+1];
	int i=0;
	while (string[i]!='\0'){
		p1->instruction[i]=string[i];
		i++;
	}
	p1->instruction[i]='\0'; 
	p1->number=number;
	p1->length=length;
	p1->first=flag;
	p1->padding=false;
	p1->pointer=NULL;
}

void PolyUnpack::FindPadding() const{
	InstructionList* p1=start;
	InstructionList* p2=NULL;
	while (p1!=NULL){
		if (strcmp(p1->instruction, "add [eax],al")==0){
			if (p2==NULL)
				p2=p1;
		}
		else{
			p2=NULL;
		}
		p1=p1->pointer;
	}
	p1=p2;
	while (p1!=NULL){
		p1->padding=true;
		p1=p1->pointer;
	}
}

void PolyUnpack::StaticAnalise(const char* FileName, const int key){
	if (key==1){
		printf("\n\n\nStart of Static Analize!\n\n\n");
	}
	INSTRUCTION inst;
	INSTRUCTION* inst_pointer=NULL;
	InstructionList* p1=NULL; 
	char *data=NULL;
	int i=0, c=0, bytes=8, len=0, number=0;
	unsigned int size=0;
	unsigned int entry_point=0;
	char string[32];
	bool flag=false;
	//PE_Reader reader(FileName);
	
	Raw_Reader reader(FileName);
	reader.LoadForStaticAnalise(data, size, entry_point);
	code_size=size;
	if(size==0){
		printf("There is no code section in this file\n");
		exit(1);
	}
	while (c < size) {
		inst_pointer=&inst;
		len=get_instruction(inst_pointer, (BYTE*)(data + c), MODE_32);

		// Illegal opcode or opcode longer than remaining buffer
		
		if (!len || (len + c > size)){
			if (key==1){
				if  (!len)
					printf("Illegal opcode\n");
				printf("%.8x  ", c);
				printf("db 0x%.2x\n", *(data + c));
			}
			c++;
			continue;
		}
		if (key==1)
			printf("%.8x  ", c);
		if ((bytes)&&(key==1)){
			for (i = 0; i < ((bytes) < (len) ? (bytes) : (len)); i++)
				printf("%.2x", data[c + i]);
			printf("  ");
			for (i = (bytes) < ((len) ? (bytes) : (len)); i < bytes*2 - len; i++)
				printf(" ");
		}
		inst_pointer=&inst;
		get_instruction_string(inst_pointer, Format(FORMAT_INTEL), (DWORD)c, string, sizeof(string));
		if (key==1)
			printf("%s\n", string);
		if (c==entry_point)
			flag=true;
		else
			flag=false;
		AddInstruction(p1, string, c , len, flag);
		c=c+len;
	} 	
	FindPadding();
	delete []data;
}

bool PolyUnpack::FindFirstInstruction(InstructionList* & p) const{
	InstructionList* point=start;
	while((point!=NULL)&&(!point->first))
		point=point->pointer;
	if(point==NULL){
		p=NULL;
		return false;
	}
	else{
		p=point;
		return true;
	}
} 


void PolyUnpack::DynamicAnalise(const char* FileName, const int key){
	if (key==1){
		printf("\n\n\nStart of Dynamic Analize!\n\n\n");
	}
	int string_size=32;
	int command_size=32;
	char* string=new char[string_size];
	char* command=new char[command_size];
	char* buf=NULL;
	try{
		INSTRUCTION inst;	
		INSTRUCTION* inst_pointer=NULL;
		InstructionList* p=start;
		bool good_parse=true;
		bool valid_file=true;
		bool sys_call=false;
		int  bytes=8, c=0, len=0, flag=1;
		for (int i=0; i<command_size; i++){
			command[i]='\0';
		}

		Register reg=EIP;
		unsigned int entry_point=0;
		//PE_Reader reader(FileName);
		Raw_Reader reader(FileName);
		reader.LoadForDynamicAnalise(buf, buf_size, entry_point, base, offset);
		emulator->begin(buf, buf_size, entry_point, base);
	
		if(!FindFirstInstruction(p)){
			if (key==1)
				printf("Can't find First instruction\n");
		}

		//int kol=0;
		unsigned int old_eip_pointer;
		for(int kol=0; kol<step_kol; kol++){		
			emulator->get_command(command, command_size);			
	
			old_eip_pointer=emulator->get_register(reg);
		
			good_parse=emulator->step(); 
			
			
			if (sys_call){
				p=NULL;
				int address=emulator->get_register(reg);
				if (!is_valid_number(address-base-offset)){
					//printf("We are in sys_call");
					continue;
				}
				sys_call=false;		
			}
			
			
			inst_pointer=&inst;
			len=get_instruction(inst_pointer, (BYTE*)(command), MODE_32);
		
			if (!len || (len>command_size)){
				if (key==1){
					if (!len)
						printf("Illegal opcode\n");
					printf("%.8x  ", c);
					printf("db 0x%.2x\n", command);
				}
				emulator->jump(old_eip_pointer+1-base);
				continue;
			}
 
			inst_pointer=&inst;
			c=0;
			get_instruction_string(inst_pointer, Format(FORMAT_INTEL), (DWORD)c, string, string_size);
			
			flag=Compare(p, string, flag, old_eip_pointer);
			
			if(flag==-1){
				//exit(2);
				valid_file=false;
				invalid_instr_kol++;
				if(key==1){
					printf("????????????????????????????????????????????????\n");
				
					for (int i = 0; i < ((bytes) < (len) ? (bytes) : (len)); i++)
						printf("%.2x", command[i]);
				
					for (int i = (bytes) < ((len) ? (bytes) : (len)); i < bytes*2 - len; i++)
						printf(" ");
					printf("%.8x  ", c);
					printf("ILLEGAL = %s\n", string);
					printf("Step Number=%d\n", kol);
					printf("????????????????????????????????????????????????\n");
				}
			}
			else{			
				if (key==1){
					for (int i = 0; i < ((bytes) < (len) ? (bytes) : (len)); i++)
						printf("%.2x", command[i]);
					printf("  ");
					for (int i = (bytes) < ((len) ? (bytes) : (len)); i < bytes*2 - len; i++)
						printf(" ");
					printf("%.8x  ", c);
					printf("%s\n", string);
					printf("Step Number=%d\n", kol);
				}
			
				if ((flag==1)||(flag==2))
					emulator->jump(old_eip_pointer+len-base); ///We have to
				/// do this because sometimes eip doesn't point on proper po
				///sition even after good parse and good step. flag==1 and 
				///flag==2 means that instructions are simple, not branch/condition instruction
				if ((flag==4)||(flag==5))
					break;
				if (flag==6)
					sys_call=true;
			
			}
		
			for (int i=0; i<string_size; i++)
				string[i]='\0';
			for (int i=0; i<command_size; i++)
				command[i]='\0';
			if((p!=NULL)&&(p->padding))
				break;
			
		}
		delete []string;
		delete []command;
		if (valid_file){
			printf("This file is valid\n");
			exit(0);
		}
		else{
			BinaryDump(); ///not nessesary now, but in network traffic 
			///search is very usefull 
			printf("This is virus!\n");
			printf("Kol of invalid instructions=%d\n", invalid_instr_kol);
			exit(1);
		}
	}
	catch(...){
		if (buf!=NULL)
			delete []buf;
		if (string!=NULL)
			delete []string;
		if (command!=NULL)	
			delete []command;
		exit(1);
	}
}

void PolyUnpack::BinaryDump(){
	char* new_buf=new char[buf_size];
	emulator->read_block(new_buf, buf_size);
	//printf("%s\n", new_buf);
	delete []new_buf;
}


bool PolyUnpack::FindInstruction(InstructionList* & p, const int number) const{
	/*if (!is_valid_number(number)){
		p=NULL;
		return false;
		exit(1);
	}*/
	
	InstructionList* point=start;
	while((point!=NULL)&&(point->number<number)){
		point=point->pointer;
	}
	if ((point!=NULL)&&(point->number==number)){
		p=point;
		return true;
	}
	else{
		p=NULL;
		return false;
	}
}

bool PolyUnpack:: is_valid_number(const int number) const{

	if ((number>=0)&&(number<=buf_size)){
		return true;
	}
	return false;
}


int PolyUnpack::Compare(InstructionList* &p, const char* command, int flag, const unsigned int old_eip){
	if (root.Is_Empty()){
		//printf("root=NULL\n");
		if (p==NULL){
			if (!is_valid_number(old_eip-offset-base)){
				printf("Unvalid EIP\n");
				//exit(1);
				throw 1;
			}
			if(!FindInstruction(p, old_eip-offset-base))
				return -1;	
		}
		int iterator=0;
		InstructionList* p2=NULL;
		flag=FindNextInstruction(p, p2, command, iterator);
		if (flag==-1)
			p=NULL;
		return flag;
	}
	else{
		//printf("root!=NULL\n");
		int flag;
		Register reg=EIP;
		flag=root.Obhod(command, *this);
		unsigned int num=emulator->get_register(reg);
		if (!(is_valid_number(num-offset-base))){
			/*if (compare(command, "call")||(compare(command, "jmp")))
				printf("Unvalid EIP\n");
				exit(0);
			exit(1);*/
			if (flag!=5)
				//exit(1);
				throw 1;
		}
		FindInstruction(p, num-offset-base);
		return flag;
	}	
}


int PolyUnpack::FindNextInstruction(InstructionList* &p, InstructionList* & p2, const char* command, int & iterator){
	//printf("Instruction=%s\n", p->instruction);
	//printf("command=%s\n", command);
		
	if ((compare(p->instruction,"call"))||(compare(p->instruction,"callf"))){
		if ((!compare (command, "call"))&&(!compare (command, "callf")))
			return -1;
		if(compare(p->instruction, "callf")){
			printf("callf was found\n");
			p=NULL;
			return 0;
		}
		int i=parse(p->instruction);
		if (p->instruction[i]=='['){
			if (command[i]!='[')
				return -1;
			p=p->pointer;	
			return 2;
		}
		else{
			if(p->pointer!=NULL){
				if (iterator==0){
					return_addresses.Push(p->pointer->number);
					//return_addresses.Print();
					iterator++;
					//printf("Future EIP=%d\n", p->pointer->number+offset+0x20000000L);
				}
			}
			else
				return -1;
			if ((p->instruction[i])=='0'){
				int number=GetNumber(p->instruction+i+2);
				if (!is_valid_number(number)){
					printf("Call outside the program\n");
					//exit(1);
					throw 1;
				}
				
				if(!FindInstruction(p, number))
					return -1;
				return 3;
			}
			else{
				char* string;
				int i=parse(p->instruction);
				get_register_name(string, p->instruction+i);
				int num=emulator->get_register(string);
				delete []string;
				if(!is_valid_number(num-base-offset))
					return 5;
				if (!FindInstruction(p, num-base-offset))
					return -1; //so that this instruction will be unvalid but not next
				return 3;
			}
		}
	}
	int cond_com=condition_command(p->instruction, command);
	
	if (cond_com==1){
		if(compare(p->instruction, "jmpf")){
			printf("jmpf was found\n");
			p=NULL;
			return 0;
		} 		
		if(compare(p->instruction, "jmp")){
			int i=parse(p->instruction);
			if (p->instruction[i]=='['){
				if (command[i]!='[')
					return -1;
				p=p->pointer;	
				return 2;
			}
			if (p->instruction[i]=='0'){
				unsigned int number=GetNumber(p->instruction+i+2);	
				if (!is_valid_number(number)){
					printf("Jump outside the program\n");
					//exit (1);
					throw 1;
				}
				if(!FindInstruction(p, number))
					return -1;
				return 3;
			}
			else{
				char* string;
				get_register_name(string, p->instruction+i);
				unsigned int num=emulator->get_register(string);
				delete []string;
				if (!is_valid_number(num-base-offset))
					return 5;
				if (!FindInstruction(p, num-base-offset))
					return -1;
				return 1;
			}
		}
		else{
			int i=parse(p->instruction);
			if (p->instruction[i]=='0'){
				unsigned int number=GetNumber(p->instruction+i+2);	
				bool valid_number=is_valid_number(number);
				if (!valid_number){
					Register reg=EIP;
					unsigned int num=emulator->get_register(reg);
					if (!is_valid_number(num)){
						printf("Condition jump leads outside the program\n");
						//exit(1);
						throw 1;
					}
					printf("Condition jump may lead outside the program\n");
				}
				if ((valid_number)&&(FindInstruction(p2, number))){
					if (root.Is_Empty())
						root.Add(p, p->pointer, p2);
					else
						p=p->pointer;
				}
				else{
					if (root.Is_Empty()){
						p2=NULL;
						root.Add(p, p->pointer, p2);
					}
					else{
						p2=NULL;
						p=p->pointer;
					}
				}
				return 0;
			}
			else{
				char* string;
				get_register_name(string, p->instruction+i);
				unsigned int number=emulator->get_register(string);
				delete []string;
				bool valid_number=is_valid_number(number-base-offset);
				if (!valid_number){
					Register reg=EIP;
					unsigned int num=emulator->get_register(reg);
					if (!is_valid_number(num)){
						printf("Condition jump leads outside the program\n");
						//exit(1);
						throw 1;
					}
					printf("Condition jump may lead outside the program\n");
				}
				if ((valid_number)&&(FindInstruction(p2, number-base-offset))){
					if (root.Is_Empty())
						root.Add(p, p->pointer, p2);
					else
						p=p->pointer;
				}
				else{
					if (root.Is_Empty()){
						p2=NULL;
						root.Add(p, p->pointer, p2);
					}
					else{
						p2=NULL;
						p=p->pointer;
					}
				}
				return 0;
			}
			return 0;
		}
		return -1;	
	}
	if (cond_com==-1){
		return -1;
	}

	if (strcmp(p->instruction, command)==0){
		
		if (compare(p->instruction, "int")){
			p=NULL;
			return 6;
		} 
		 
		if ((compare(p->instruction ,"ret"))||(compare(p->instruction,"retn"))||
			(compare(p->instruction, "retf"))||(compare(p->instruction, "iret"))||
											(compare(p->instruction, "rep ret"))){
			unsigned int temp;
			if (iterator==0){
				iterator++;
				return_addresses.Pop(temp);
				if (temp==-1){
					p=NULL;
					return 4;
					//printf("Stack is empty\n");
				}
				//return_addresses.Print();
				if (!FindInstruction(p, temp)){
					return -1;
				}
				emulator->jump(temp+offset);
				return 3;
			}
		}
		p=p->pointer;
		return 1;
	}
	else{
		p=NULL;
		return -1;
	}
	throw "Bad command\n";
}
