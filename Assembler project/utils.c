#include "utils.h"
#define EXT_MAX_LEN 5

/*files functions*/
char *file_to_as(char *oldname)
{
	char *as_name;
	as_name = (char*)malloc((strlen(oldname) + 4)*sizeof(char));
	if (as_name == NULL)
	{
		printf("***ALLOCATION ERROR***\n");
		exit(0);
	}
	strcpy(as_name, oldname);
	strcat(as_name, ".as");
	return(as_name);
}

char *file_to_type(char *oldname, int type)
{
	char *type_name = (char *)malloc((strlen(oldname) + EXT_MAX_LEN)*sizeof(char));
	if (type_name == NULL)
	{
		printf("***ALLOCATION ERROR***\n");
		exit(0);
	}
	strcpy(type_name, oldname);

	switch (type)/*switch case that add the fit end each file*/
	{
	case ob:
		strcat(type_name, ".ob");
		break;
	case am:
		strcat(type_name, ".am");
		break;
	case ent:
		strcat(type_name, ".ent");
		break;
	case ext:
		strcat(type_name, ".ext");
		break;
	}
	
	return type_name;
}

/*helper functions*/
int isRegister(char *token)
{
	if (strlen(token) == MAX_REG_NAME && token[0] == 'r' && token[1] >= '0' && token[1] <= '7')
		return 1;
	return 0;
}

int sumOperands(int type, int isSrc, int isDest, int pass)
{
	int sum = isDest + isSrc;/*sum of operands*/
	if (type == mov || type == cmp || type == add || type == sub || type == lea)
	{
		if (pass == PASS2 || (pass == PASS1 && sum == 2))
		{
			return sum;
		}
	}
	if (type == not || type == clr || type == inc || type == dec || type == jmp || type == bne || type == get || type == prn || type == jsr)
	{
		if (pass == PASS2 || (pass == PASS1 && sum == 1))
		{
			return sum;
		}
	}
	if (type == rts || type == hlt)
	{
		if (pass == PASS2 || (pass == PASS1 && sum == 0))
		{
			return sum;
		}
	}
	printf("Error in line %d: The number of operands does not match the command requirements.\n", lineNum);
	return -1;
}

int methodType(char* param)
{
	char* temp = '\0';
	if (param == NULL || param == '\n') { return unknownM; }
	if (param[0] == '#')
	{
		param++;
		if (isNum(param))
		{
			return Immediate;
		}
		printf("Syntax error in line %d: error in the syntax of the addressing method.\n", lineNum);
		return unknownM;
	}
	else
	{
		if (isRegister(param) || isParamLabel(param))
		{
			if (isRegister(param)){
				return Register;
			}
			else
				return Direct;
		}
		else
		{
			temp = strtok(param, ".");
			if (isParamLabel(temp))
			{
				temp = strtok(NULL,"\n\t");
				if (!strcmp(param, "1") || !strcmp(param, "2"))
				{
					return To_struct;
				}
				else{
					printf("Syntax error in line %d: error in the syntax of the addressing method of struct.\n", lineNum);}
			}
			else {
				printf("Error in line %d: A recognized address method was not received.\n", lineNum);}
			return unknownM;
		}
	}
}

void splittingCommas(char line[], char strStorage[], char *tmpLine, int quMarks)
{
	int i, j, k;
	int temp = -1;
	if (quMarks > 1)
	{
		temp = quMarks;
		k = 0;
	}
	for (i = 0, j = 0; line[i] != '\0' && line[i] != EOF; i++, j++)
	{
		if (line[i] == '"')/*this mean that we in a string param*/
		{
			temp--;
			tmpLine[j] = line[i];
			strStorage[k] = line[i];/*stored the original string param before the spliiting*/
			k++;
		}
		if (temp <= 0 || temp == quMarks)
		{
			if (temp == 0)
			{
				strStorage[k] = '\0';
			}

			if (!isspace(line[i]))
			{
				if (line[i] == ',')
				{
					tmpLine[j] = ' ';
					j++;
					tmpLine[j] = line[i];
					j++;
					tmpLine[j] = ' ';
				}
				else
					tmpLine[j] = line[i];
			}
			else
			{
				tmpLine[j] = line[i];
			}
		}
		else
		{
			if (isspace(line[i]))
			{
				tmpLine[j] = '.';/*We insert a tenporery meaningless character for divisions the line to word without destrooy this param*/
				strStorage[k] = line[i];
				k++;
			}
			else if (line[i] != '"')
			{
				tmpLine[j] = line[i];
				strStorage[k] = line[i];
				k++;
			}
		}
	}
	tmpLine[j] = '\0';
}

int wordsInLine(char line[])
{
	char* str = (char *)malloc(strlen(line) +1);
	char* token = '\0';
	int count = 0;
	strcpy(str, line);
	token = strtok(str, SPACE);
	while (token) 
	{
		token = strtok(NULL, SPACE);
		++count;
	}
	free(str);
	return count;
}

char *cleanTrailingSpaces(char * _str)
{
	char *end = _str + strlen(_str) - 1;
	while (end > _str && isspace((unsigned char)*end))
		end--;

	end[1] = '\0';
	return _str;
}

char** separatingWords(char line[], char** arr, int count, char* s_arr)
{
	int index = 0;
	int ind = -1;
	char *curr = strtok(line, SPACE);
	while (index < count && curr != NULL)
	{
		if (!isspace(curr[0]))
		{
			if (curr[0] == '"')
			{
				ind = index;
				arr[index] = curr;
				index++;
				curr = strtok(NULL, SPACE);
			}
			else
				arr[index] = curr;
			index++;
			curr = strtok(NULL, SPACE);
		}
		else
			curr = strtok(NULL, SPACE);
	}
	if (ind > -1)
	{
		arr[ind] = s_arr;
		return arr;
	}
	return arr;
}

int isAlphanumaricOrNumbers(char* name)
{
	int i = 0;
	if (!isalnum(name[i]))
	{
		return 0;
	}
	for (i = 1; name[i] != '\0'; i++)
	{
		if (!isalnum(name[i]) && !isdigit(name[i]))
		{
			return 0;
		}
	}
	return 1;
}

int isSaveWord(char* word)
{
	if (isRegister(word) || findCommand(word) != unknownComm || !strcmp(word, "extern") || !strcmp(word, "entry") || !strcmp(word, "struct") || !strcmp(word, "data") || !strcmp(word, "string") || !strcmp(word,"macro") || !strcmp(word,"endmacro"))
	{
		return 1;
	}
	return 0;
}

int validSyntaxLabel(char* name, Bool extenral)
{
	if (isSaveWord(name))
	{
		if (extenral)/*if its check for the user we print error*/
		{
			printf("Error in line %d: illegal name - label name cannot be a save word.\n", lineNum);
		}
		return 0;
	}
	if (!isAlphanumaricOrNumbers(name))
	{
		if (extenral)
		{
			printf("Error in line %d: illegal name - label name must begin with alphanumeric letter and followed by a series of alphanumeric letters or numbers.\n", lineNum);
		}
		return 0;
	}
	return 1;
}

int isDefineSymbol(char* sym, struct symbolTable *labelList)
{
	char sname[MAX_LEN_LABEL+1];
	int len = strlen(sym) - 1;
	if (sym[len] == ':')/*A valid defenition of label must end with ":"*/ 
	{
		if ((strlen(sym) - 2) <= MAX_LEN_LABEL)
		{
			strcpy(sname , strtok(sym, ":"));
			if (!isExistSym(labelList, sym, FALSE)) /* If lebel name didnt exist it is possible to define her*/
			{
				return (validSyntaxLabel(sname, TRUE));
			}
			printf("Error in line %d: illegal name - label name cannot be define twice, there is an label with the same name.\n", lineNum);
		}
		else
			printf("Error in line %d: illegal name - label name cannot be longer than 30 chars.\n", lineNum);
	}
	return 0;
}

int getNumOfOpsByCmd(char *command)
{
	static const int commandToNumOfOperands[16] = {2, 2, 2, 2, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 0, 0};
	int commandEnum = unknownComm;

	if (!command)
		return unknownComm;

	commandEnum = findCommand(command);
	return (commandEnum != unknownComm) ? commandToNumOfOperands[commandEnum] : unknownComm;
}

int findCommand(char command[])
{
	if (!strcmp(command, "mov")) { return mov; }
	if (!strcmp(command, "cmp")) { return cmp; }
	if (!strcmp(command, "add")) { return add; }
	if (!strcmp(command, "sub")) { return sub; }
	if (!strcmp(command, "not")) { return not; }
	if (!strcmp(command, "clr")) { return clr; }
	if (!strcmp(command, "lea")) { return lea; }
	if (!strcmp(command, "inc")) { return inc; }
	if (!strcmp(command, "dec")) { return dec; }
	if (!strcmp(command, "jmp")) { return jmp; }
	if (!strcmp(command, "bne")) { return bne; }
	if (!strcmp(command, "get")) { return get; }
	if (!strcmp(command, "prn")) { return prn; }
	if (!strcmp(command, "jsr")) { return jsr; }
	if (!strcmp(command, "rts")) { return rts; }
	if (!strcmp(command, "hlt")) { return hlt; }
	else
		return unknownComm;
}

int QuMarks(char line[])
{
	int quMark = 0;
	int i;
	for (i = 0; line[i] != '\n'; i++)
	{
		if (line[i] == '"')
		{
			quMark++;
		}
	}
	return quMark;
}

int isValidLen(char line[])
{
	int i, comma = 0, result = 0, valid = FALSE;
	for (i = 0; i < MAX_LENGTH +1; i++)
	{
		if(line[i] == '\n')/*this is check a valid len length*/
		{
			valid = TRUE;
			break;
		}
		if (isspace(line[i]))
		{
			continue;
		}
		else
		{
			if (line[i] == ',')
			{
				result++;
				comma++;
			}
			else
				result++;
		}
	}

	if(!valid)
	{
		return -1;
	}
	if(valid && result == 0)
	{
		return -2; /* indication for empty but legal line */
	}
	return comma;
}

int isExistSym(struct symbolTable* table, char sname[], int isEntry)
{
	struct symbolNode *node = table->head;
	while (node != NULL)  /*Run on all the symbols we have already found*/
	{
		if (!strcmp(node->s_name, sname))
		{
			if (isEntry)
			{
				if (node->s_type == EXTERN)
				{
					printf("Error in line %d: entry cant be extern!\n", lineNum);
					return 0;
				}
				node->s_type = ENTRY;
			}
			return 1;
		}
		node = node->next;
	}
	if (isEntry)
	{
		printf("Error in line %d: entry label doesnt exist.\n", lineNum);
	}
	return 0;
}

int labelType(char type[]) /* Returns which prompt is in between string, extern, data, code and entry */
{
	if (!strcmp(type, ".data") || !strcmp(type, ".string") || !strcmp(type, ".struct"))
		return IS_DIRECTIVE;
	if (!strcmp(type, ".entry"))
		return ENTRY;
	if (!strcmp(type, ".extern"))
		return EXTERN;
	if (findCommand(type) >= 0)
		return IS_COMMAND;
	return -1;
}

void insertSymbol(struct symbolTable *_table, char* _name, int _type, int _value)
{
	struct symbolNode *newSymbol = (symbolNode*)malloc(sizeof(symbolNode));
	struct symbolNode *ptr = NULL;
	strcpy(newSymbol->s_name, _name);
	newSymbol->s_type = _type;
	newSymbol->s_value = _value;
	newSymbol->next = NULL;
	ptr = _table->head;
	if (!_table->head)
	{
		_table->head = newSymbol;
	}
	else
	{
		ptr = _table->head;
		_table->head = newSymbol;
		newSymbol->next = ptr;
	}
}

int isNum(char* param)
{
	if (!atoi(param))
	{	
		return 0;
	}

	return 1;
}


int isParamLabel(char param[])
{
	if ((strlen(param) - 1) <= MAX_LEN_LABEL)
	{
		return validSymName(param, FALSE);
	}
	return 0;
}

void freeSymList(struct symbolTable* _table)
{
    if (!_table)
        return;
    freeSymNodes(_table->head);
    free(_table);
}


void freeSymNodes(struct symbolNode* _head)
{
	/* stop condition for the recursion */
    if (!_head)
		return;
	/* use the recursion to advance until the end of the list*/
	freeSymNodes(_head->next);
	/* when the recursion fold back, we can free the current node */
	free(_head);
}

void freeExtList(struct entExtList *extHead)
{
    struct entExtList *tmp = extHead;
    extHead = extHead->next;
    free(tmp);
}


