#ifndef __FIRSTPASS__
#define __FIRSTPASS__

#include "macro.h"


/*First pass functions*/

int extAndEntSyntax(char** arr, int i, int extOrEnt, int wasLabel);

int commandMethods(int commandType, int source, int dest);

int isCommand(int type, char** arr, int i);

int numWords(int isSrc, int isDest, int src, int dest);

void updateSym(struct symbolTable* head);

int encodeData(int word);

int insertStruct(char** arr, int i, int arr_size);
int insertData(char** arr, int i, int arr_size);
int insertString(char** arr, int i, int arr_size);
void stringToBin(char str[]);
int isValidStr(char str[]);

/*first pass mainly function*/
struct symbolTable * firstPass(FILE* fp);
struct symbolTable * _firstPass(FILE* fp);

#endif
