#include "macro.h"

void freeMacroList(struct macro *head)
{
    struct macro *tmp = head;
    head= head->next;
    free(tmp);
}

int macroCall(char* word, FILE* fptrAm, struct macro* current)
{
	
	while (current->next != NULL)
	{
		if (!strcmp(current->mname, word))
		{
			fprintf(fptrAm, "%s", current->mdata);
			return 1;
		}
		current = current->next;
	}
	return 0;
}


void addData(FILE* fptrAs, struct macro *temp)
{
    size_t sizeOfAllocation;
	char line[MAX_LENGTH + 3];
    temp->mdata = (char*)calloc(1, sizeof(char));
	while(strstr(fgets(line, 83, fptrAs), "endmacro") == NULL)
	{
	    sizeOfAllocation = (temp->mdata) ? strlen(temp->mdata) + strlen(line) + 1 : strlen(line) + 1;
 temp->mdata = (char*)realloc(temp->mdata, sizeOfAllocation*sizeof(char));
    	strcat(temp->mdata, line);
	}

}

void insertNode(FILE* fptrAs,macro **macroHead, char *word)
{
	macro *temp = (macro*)malloc(sizeof(macro));
	strcpy(temp->mname, word);
	addData(fptrAs, temp);
	temp->next = *macroHead;
	*macroHead = temp;
}

int isValidName(char *name, struct macro *node)
{
	if ((strlen(name) - 1) <= MAX_LEN_MACRO)
	{
		if (isSaveWord(name) || !isAlphanumaricOrNumbers(name))
		{
			if (isSaveWord(name))
			{
				printf("Error in line %d: macro name cant be a save word.\n", lineNum);
			}
			else
				printf("Error in line %d: macro name must begin with alphanumeric letter and followed by alphanumeric letters or numbers.\n", lineNum);
			return 0;
		}
		while (node != NULL)  /*Run on all the symbols we have already found*/
		{
			if (!strcmp(node->mname, name))
			{
				printf("Error in line %d: macro name cannot be define twice, there is an macro with the same name.\n", lineNum);
				return 0;
			}
			node = node->next;
		}
		return 1;
	}
	printf("Error in line %d: is not a macro - macro name cannot be longer than 30 chars.\n", lineNum);
	return 0;
}

int preAssembler(FILE* fptrAs, FILE* fptrAm)
{
	int inMacro, isMacroCall, error = 0;
	char* word = NULL;
	char *nameMac = '\0';
	char line[MAX_LENGTH+3], realLine[MAX_LENGTH+3];
	char *prev = "";
	struct macro *macroHead =(macro*)calloc(1,sizeof(macro));
	macroHead->next= NULL;
	lineNum = 0;
	while(fgets(line, MAX_LENGTH+3, fptrAs))
	{
		strcpy(realLine, line);
		lineNum++;
		isMacroCall = 0, inMacro = 0;
		word = strtok(line, SPACE);
		while (word != NULL)
		{
			if(strcmp(word,SPACE))
			{
				if (isMacroCall == 1 || inMacro == 1)
				{
					if (isMacroCall == 1)
					{
						printf("syntax error in line %d: in macro call line only macro name need to show\n", lineNum);
					}
					else
						inMacro = -1;
					printf("syntax error in line %d: in define macro line need be only the define (macro and name)\n", lineNum);
					error = 1;
					break;
				}
				if(macroCall(word, fptrAm, macroHead) == 1)
				{
					isMacroCall = 1;
				}
				if(!strcmp(prev, "macro"))
				{	
					inMacro = 1;
					prev = "";
					nameMac = word;
				}
				else
				{
					if (!strcmp("macro", word))
					{
						prev = "macro";
					}
				}
			}
			word = strtok(NULL, SPACE);
		}

		if(inMacro == 1)
		{	
			if(isValidName(nameMac, macroHead))
			{	
				insertNode(fptrAs, &macroHead, nameMac);
			}
			else
				error = 1;
			continue;
		}
		else if(!strcmp(prev,"macro") && inMacro == 0)
		{
			printf("error in line %d: enxepted name macro to define\n", lineNum);
			error = 1;
		}
		else
		{
			if (strstr(line, realLine) == NULL && isMacroCall == 0)
			{
				fprintf(fptrAm, "%s", realLine);
			}
		}
	}
	freeMacroList(macroHead);
	if (error == 1)
	{
		return 0;
	}
	return 1;
}
