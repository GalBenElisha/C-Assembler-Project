#include "FirstPass.h"

/*First pass functions*/

int extAndEntSyntax(char** arr, int i, int extOrEnt, int wasLabel)
{
	char name[7];
	if(extOrEnt == EXTERN)
	{
		strcpy(name, "extern");
	}
	else{strcpy(name,"entry");}

	if(wasLabel == TRUE)
	{
		printf("Warning in line %d: label cant be before an %s diractive.\n", lineNum,name);
	}
	if (!isParamLabel(arr[i]))
	{
			printf("Error in line %d: The %s directive received an invalid operand ,it must receive a valid label.\n", lineNum, name);
			return 0;
	}
	if(arr[i] == NULL)
	{
			printf("Erorr in line %d: The %s directive is empty, after %s must be a label.\n", lineNum, name);
			return 0;
	}
	if (arr[i+1] != NULL)
	{
			printf("Error in line %d: The %s directive received too many operands ,it must receive only one operand.\n", lineNum, name);
			return 0;
	}
	return 1;
}

int commandMethods(int commandType, int source, int dest)
{
	if (commandType == mov || commandType == add || commandType == sub)
	{
		return ((source == Immediate || source == Direct || source == To_struct || source == Register)
			&& (dest == Direct || dest == To_struct || dest == Register));
	}
	if (commandType == cmp)
	{
		return ((source == Immediate || source == Direct || source == To_struct || source == Register)
			&& (dest == Immediate || dest == Direct || dest == To_struct || dest == Register));
	}
	if (commandType == lea)
	{
		return (source == Direct || source == To_struct)
			&&
			(dest == Direct || dest == To_struct || dest == Register);
	}
	if (commandType == not || commandType == clr || commandType == inc || commandType == dec
		|| commandType == jmp || commandType == bne || commandType == get || commandType == jsr)
	{
		return (dest == Direct || dest == To_struct || dest == Register);
	}
	if (commandType == prn)
	{
		return (dest == Immediate || dest == Direct || dest == To_struct || dest == Register);
	}
	if (commandType == rts || commandType == hlt)
	{
		return 1;
	}
	return 0;
}

int isCommand(int type, char** arr, int i)
{
	int haveSource = 0, haveDest = 0, source = 0, dest = 0;
	if (arr[i] != NULL)
	{
		haveSource = 1;
		source = methodType(arr[i]);
		i++;
		if (arr[i] != NULL)
		{
			if (strcmp(arr[i], ","))
			{
				printf("Syntax error in line %d: There is an error in the syntax of the line, a missing comma between two operands.\n", lineNum);
				return 0;
			}
			else
			{
				i++;
				if (arr[i] != NULL)
				{
					haveDest = 1;
					dest = methodType(arr[i]);
					if (arr[i+1] != NULL)
					{
							printf("Error in line %d: Too many command operands received.\n", lineNum);
							return 0;
					}
				}
				else{
				printf("Syntax error in line %d: The second operand not accepted after the comma at the end of line.\n", lineNum);
				return 0;}
			}
		}
		else
		{
			haveDest = 1,haveSource = 0, dest = source, source = 0;}
	}
	if (dest == unknownM || source == unknownM)
	{
		return 0;
	}
	else
	{
		if (sumOperands(type, haveSource, haveDest, PASS1) != -1)
		{
			if (commandMethods(type, source, dest))
			{
				return (numWords(haveSource, haveDest, source, dest));
			}
			else
				printf("Error in line %d: The received addressing methods do not fit the requirements of the command.\n", lineNum);
		}
		else
			printf("Error in line %d: The number of operands that received does not match the command requirements.\n", lineNum);
		return 0;
	}
}

int numWords(int isSrc, int isDest, int src, int dest)
{
	int count = 1;
	if (isSrc == 1 && isDest == 1 && src == Register && dest == Register)
	{
		count++;
		return count;
	}
	if (isSrc == 1)
	{
		if (src == To_struct)
		{
			count += 2;
		}
		else
			count++;
	}
	if (isDest == 1)
	{
		if (dest == To_struct)
		{
			count += 2;
		}
		else
			count++;
	}
printf("count = %d\n",count);
	return count;
}

void updateSym(struct symbolTable* labelList)
{
	struct symbolNode* node = labelList->head;
	while (node != NULL)
	{
		if (node->s_type == IS_DIRECTIVE)
		{
			node->s_value += ic;
		}
		node = node->next;
	}
}

int encodeData(int word)
{
	if (word <= 1023 && word >= -1023)
	{
		data[dc++] = word;
return 1;
	}
		printf("Error in line %d: the encode of data word too bigg , because she over the 10 bits in binary.\n", lineNum);
return 0;
}

int insertStruct(char** arr, int i, int arr_size)
{
	if (i == arr_size || !strcmp(arr[i], ","))
	{
		if (i == arr_size)
		{
			printf("Error in line %d: The directive statement is empty, there is no parameters.\n", lineNum);
		}
		else
			printf("Syntax error in line %d: A comma is found before the first parameter.\n", lineNum);
		return 0;
	}
	if (isNum(arr[i]))
	{
		if(!encodeData(atoi(arr[i]))){return 0;}
		i++;
		if ((i != arr_size) && (!strcmp(arr[i], ",")))
		{
			i++;
			if (i != arr_size)
			{
				if (isValidStr(arr[i]))
				{
					stringToBin(arr[i]);
				}
				else
					if (!isValidStr(arr[i])) {
						printf("Error in line %d: By definition a valid string must start and end with quotes, and this case is worng.\n", lineNum);
					}
				return isValidStr(arr[i]);
			}
			else
				printf("Error in line %d: No string type parameter entered.\n", lineNum);
			return 0;
		}
		else
			printf("Syntax error in line %d: Missing comma between two parameters.\n", lineNum);
		return 0;
	}
	else
	{
		if (!isNum(arr[i])) {
			printf("Syntax error in line %d: The parameter dont match the directive statement definition.\n", lineNum);
		}return isNum(arr[i]);
	}
	i++;
	if (i > MAX_STRUCT_PARAMS)
	{
		printf("Error in line %d: Too many parameters, in this directive sentence there should be only 2 parameters.\n", lineNum);
		return 0;
	}
	return 1;
}

int insertData(char** arr, int i, int arr_size)
{
	int j, flag = 0;
	if (i == arr_size || !strcmp(arr[i], ","))
	{
		if (i == arr_size)
		{
			printf("Error in line %d: The directive statement is empty, there is no parameters.\n", lineNum);
		}
		else
			printf("Syntax error in line %d: A comma is found before the first parameter.\n", lineNum);
		return 0;
	}
	if (i == arr_size - 1)
	{
		if (!isNum(arr[i]))
		{
			printf("Error in line %d: Unrecognized parameter, this type doesnt fit the data definition, the parameter should be integer.\n", lineNum);
			return 0;
		}
		else
			if(!encodeData(atoi(arr[i]))){return 0;}
		return isNum(arr[i]);
	}
	for (i = i, j = i + 1; j < arr_size && i < arr_size; i = i + 2, j = j + 2)
	{
		if (isNum(arr[i]))
		{
			if (isNum(arr[j]) || (!strcmp(arr[j], ",") && j == arr_size - 1))
			{
				if (j == arr_size - 1)
				{
					printf("Syntax error in line %d: A comma is found after the last parameter.\n", lineNum);
				}
				else
					printf("Syntax error in line %d: A missing comma was found between two parameters.\n", lineNum);
				return 0;
			}
			if (!strcmp(arr[j], ","))
			{
				if(!encodeData(atoi(arr[i]))){return 0;}
				if (flag == 0 && (arr_size - 2 == j || arr_size - 2 == i))
				{
					flag = 1;
					if(arr_size - 2 == j) { j = j - 2; }
					else
						i = i - 2;
				}
			}
			else{
				printf("Syntax error in line %d: Unrecognized parameter that dont fit the directive statement definition.\n", lineNum);
				return 0;
			}
		}
		else
		{
			if (!strcmp(arr[i], ",")) { printf("Syntax error in line %d: Duplicate commas found.\n", lineNum); }
			else
				printf("Syntax error in line %d: Unrecognized parameter that dont fit the directive statement definition.\n", lineNum);
			return 0;
		}
	}
	
	return 1;
}

int insertString(char** arr, int i, int arr_size)
{
	if (i == arr_size)
	{
		printf("Error in line %d: The directive statement is empty, there is no parameters.\n", lineNum);
		return 0;
	}
	if (isValidStr(arr[i]))
	{
		if (i == arr_size - 1)
		{
			stringToBin(arr[i]);
			return 1;
		}
		else
			printf("Error in line %d: Too many parameters, in this directive sentence there should be only 1 parameter.\n", lineNum);
		return 0;
	}
	else
		printf("Error in line %d: By definition a valid string must start and end with quotes, and this case is worng.\n", lineNum);
	return 0;
}

void stringToBin(char str[])
{
	int len = strlen(str);
	int i = 0;
	for (; i < len; i++)
	{
		if ((str[i] == '"' && i == 0) || (str[i] == '"' && i == len - 1))
		{
			continue;
		}
		encodeData((int)str[i]);
	}
	encodeData(0);
}

int isValidStr(char str[])
{
	int length = strlen(str);
	if (str[0] == '"' && str[length - 1] == '"')
	{
		return 1;
	}
	return 0;
}


struct symbolTable * firstPass(FILE* fptrAm)
{
	int symbol = FALSE, i = 0, error = FALSE;
	int commaCount = 0, quMarks = 0, wordCount = 0;
	char line[MAX_LENGTH + 2], strStorage[MAX_LENGTH + 2], sname[MAX_LEN_LABEL + 2];
	char *tmpLine = NULL;
	char **arr = NULL;
	symbolTable *labelList = (symbolTable*)malloc(sizeof(symbolTable));
	labelList->head = NULL;
	labelList->last = labelList->head;	
	ic = 0, dc = 0, lineNum = 0;
	while(fgets(line, MAX_LENGTH + 2, fptrAm))
	{
		i = 0;
		lineNum ++;
		commaCount = isValidLen(line);
		if (commaCount == -1 || commaCount == -2)
		{
			if(commaCount == -1)
			{
				printf("Error in line %d: invalid line.\n", lineNum);
				error = TRUE;
			}
			continue;
		}
		
		tmpLine = (char*)malloc((commaCount * 2) + (strlen(line)+1)*sizeof(char));
		if (tmpLine == NULL)
		{
			printf("Memory not allocated.\n");
			exit(0);
		}
		
		quMarks = QuMarks(line);
		splittingCommas(line, strStorage, tmpLine, quMarks);
		tmpLine = cleanTrailingSpaces(tmpLine);
		wordCount = wordsInLine(tmpLine);
		arr = (char**)calloc((wordCount*(strlen(line)+1)),sizeof(char));
		if (arr == NULL)
		{
			printf("Memory not allocated.\n");
			exit(0);
		}
		separatingWords(tmpLine, arr, wordCount, strStorage);
		if(isDefineSymbol(arr[i], labelList))
		{
			symbol = TRUE;
			strcpy(sname, arr[i]);
			i++;
		}

		switch (labelType(arr[i]))
		{
			case IS_DIRECTIVE:
				if (symbol == TRUE)
				{	
					insertSymbol(labelList, sname, IS_DIRECTIVE, dc);
				}
				if((!strcmp(arr[i], ".data") && !insertData(arr, i + 1, wordCount)) 
				|| (!strcmp(arr[i], ".string") && !insertString	(arr, i + 1, wordCount))
				|| (!strcmp(arr[i], ".struct") && !insertStruct(arr, i + 1, wordCount)))
				{
					error = TRUE;
				}
				break;
			
			case ENTRY:
				if(!extAndEntSyntax(arr, i+1, ENTRY, symbol))
				{
					error = TRUE;
				}
				break;		
			
			case EXTERN:
				i++;
				if(extAndEntSyntax(arr, i, EXTERN, symbol))
				{
					insertSymbol(labelList, arr[i], EXTERN, 0);
				}
				else
				{error = TRUE;}
				break;

			case IS_COMMAND:
				if (findCommand(arr[i]) == -1)
				{
					error = TRUE;
					printf("Error in line %d: Invalid command name.\n", lineNum);
				}
				else
				{
					if (symbol == TRUE)
					{
						insertSymbol(labelList, sname, IS_COMMAND, ic);
					}
					ic += isCommand(findCommand(arr[i]), arr, i+1);
				}
				break;
		}/*end of switch*/	
		free(tmpLine);
		free(arr);
		symbol = FALSE;
		memset(sname, '\0', MAX_LEN_LABEL+1);
		memset(strStorage, '\0', MAX_LENGTH + 2);
	}
	
	if (!error)
	{ 	ic+=100;
		updateSym(labelList);
		return labelList;
	}
	printf("Errors were found in the first pass, so we will not proceed to the second pass.\n");
	freeSymList(labelList);
	return NULL;
}
