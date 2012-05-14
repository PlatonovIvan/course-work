#include <stdio.h>
#include <string.h>
#include "Instruction_Tree.h"
#include "PolyUnpack.h"



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
	//Obhod2(root);
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
				//printf("po1->instruction=%s\n", po1->instruction);
				//printf("po2->instruction=%s\n", po2->instruction);
				//printf("po3->instruction=%s\n", po3->instruction);
				Add(po1, po2, po3);
				//Obhod2(root);
			}
		}
		return result;
}

void Instruction_Tree::Obhod(Instruction_Leaf* & p1, const char* command,
							InstructionList* & original, PolyUnpack & unpack, 
									int & kol, int & iterator, int & result){
	Instruction_Tree* p2;
	if ((p1->left!=NULL)&&(p1->right!=NULL)){
		if (p1->left->valid!=-1){
			Obhod(p1->left, command, original, unpack, kol, iterator, result);
		}
		if (p1->right->valid!=-1){
			Obhod(p1->right, command, original, unpack, kol, iterator, result);
		}
		if ((p1->left->valid==-1)&&(p1->right->valid==-1))
			p1->valid=-1;
		return;
	}
	if ((p1->left!=NULL)&&(p1->right==NULL)){
		if (p1->left->valid!=-1){
			Obhod(p1->left, command, original, unpack, kol, iterator, result);
		}
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
			//kol++;
			InstructionList* po2=p1->pointer;
			InstructionList* po3=NULL;
			int flag;
			flag=unpack.FindNextInstruction(po2, po3, command, iterator);
			
			if (result<flag){
				result=flag;
			}
			
			if (flag==-1){
				//printf("flag=-1\n");
				//printf("%s\n", po2->instruction);
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
			if((flag==4)||(flag==5)){
				Destructor(root);
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
					//printf("Right Leaf %s\n", po3->instruction);
					p1->right=new Instruction_Leaf;
					p2=p1->right;
					p2->pointer=po3;
					p2->valid=0;
					p2->left=NULL;
					p2->right=NULL;
				}
				else {
					//printf("Right Leaf=NULL\n");
				}
				kol++;
			}
		}
	}
}


void Instruction_Tree::Obhod2(Instruction_Leaf* & p1){
	Instruction_Tree* p2;
	if ((p1->left!=NULL)&&(p1->right!=NULL)){
		//printf("(p1->left!=NULL)&&(p1->right!=NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		
		Obhod2(p1->left);
		Obhod2(p1->right);

		return;
	}
	if ((p1->left!=NULL)&&(p1->right==NULL)){
		//printf("(p1->left!=NULL)&&(p1->right==NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		
		Obhod2(p1->left);
		
		return;
	}
	if ((p1->left==NULL)&&(p1->right!=NULL)){
		//printf("(p1->left==NULL)&&(p1->right!=NULL)\n");
		//printf("instruction=%s\n valid=%d\n", p1->pointer->instruction, p1->valid);
		
		Obhod2(p1->right);
		
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
	//printf("Destructor\n");
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
