#ifndef __MACRO__
#define __MACRO__

#include "utils.h"

typedef struct macro 
{
	char mname[MAX_LEN_MACRO + 1];
	char* mdata;
	struct macro* next;
}macro;

void freeMacroList(struct macro *head);
int macroCall(char* word, FILE* fptrAm, struct macro* current);
void addData(FILE* fptrAs, struct macro *temp);
void insertNode(FILE* fptrAs, struct macro **head, char *word);
int isValidName(char *name, struct macro *head);
int preAssembler(FILE* fptrAs, FILE* fptrAm);

#endif
