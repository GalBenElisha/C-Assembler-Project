#ifndef __UTILS__
#define __UTILS__
#include "header.h"

#define TO_BOOL(X)  (X)? TRUE:FALSE;

typedef char Bool;

/*files functions*/

char* file_to_as(char *oldname);

char* file_to_type(char *oldname, int type);

/*other functions*/

int isRegister(char *token);

int sumOperands(int type, int isSrc, int isDest, int pass);

int methodType(char* param);

void splittingCommas(char line[], char strStorage[], char* tmpLine, int quMarks);

int wordsInLine(char line[]);

char** separatingWords(char line[], char* arr[], int count, char* s_arr);

int isAlphanumaricOrNumbers(char* name);

int isSaveWord(char* word);

int validSyntaxLabel(char* name, bool extenral);

int isDefineSymbol(char* sym, struct symbolTable* head);

char *cleanTrailingSpaces(char * _str);

int findCommand(char command[]);

int QuMarks(char line[]);

int isValidLen(char line[]);

int isExistSym(struct symbolTable* table, char sname[], int isEntry);

int labelType(char type[]);

int getNumOfOpsByCmd(char * command);

void insertSymbol(struct symbolTable *head, char *name, int type, int value);

int isNum(char param[]);

int isParamLabel(char param[]);

void freeSymList(struct symbolTable *head);

void freeSymNodes(struct symbolNode* _head);

void freeExtList(struct entExtList *e_Head);

#endif

