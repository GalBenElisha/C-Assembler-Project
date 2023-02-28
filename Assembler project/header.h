#ifndef __HEADER__
#define __HEADER__

#include "assembler.h"

int instructions[MAX_MEMORY_SIZE];
int data[MAX_MEMORY_SIZE];
int ic, dc;
int lineNum;

typedef struct symbolNode
{
	char s_name[MAX_LEN_LABEL + 1];
	int s_type;
	int s_value;
	struct symbolNode *next;
}symbolNode;

typedef struct symbolTable
{
	symbolNode * head;
	symbolNode * last;
}symbolTable;

typedef struct entExtList
{
	char name[MAX_LEN_LABEL + 1];
	int address;
	int entOrExt;
	struct entExtList *next;
}entExtList;



#endif
