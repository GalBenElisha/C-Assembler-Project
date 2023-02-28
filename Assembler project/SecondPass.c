#include "SecondPass.h"

void create_files(char *sourceName, struct symbolTable *symTable, struct entExtList *e_Head, int entry)
{
	FILE *fptrOb, *fptrEnt, *fptrExt;
	char *filename = (char*)malloc((strlen(sourceName)+5)*sizeof(char));
	filename = file_to_type(sourceName, ob);
	fptrOb = fopen(filename, "w");
	if(fptrOb == NULL)
	{
		printf("Error opening file !\n");
		exit(1);
	}
	create_ob(fptrOb);
	fclose(fptrOb);
	
	if (e_Head != NULL)/* בדיקה האם צריך קובץ אקסטרן */
	{
		filename = file_to_type(sourceName, ext);
		fptrExt = fopen(filename, "w");
		if(fptrExt == NULL)
		{
			printf("Error opening file !\n");
			exit(1);
		}
		create_ext(fptrExt,e_Head);
		fclose(fptrExt);
	}
	if(entry == TRUE)/* בדיקה האם צריך קובץ אנטרי */
	{
			filename = file_to_type(sourceName, ent);
			fptrEnt = fopen(filename, "w");
			if (fptrEnt == NULL)
			{
				printf("Error opening file !\n");
				exit(1);
			}
			create_ent(fptrEnt, symTable);
			fclose(fptrEnt);
	}
	free(filename);
	return;
}

/*create files functions*/

void toBase32(char* converted,int num)
{
    const char base32[] = "!@#$%^&*<>abcdefghijklmnopqrstuv";
    unsigned int left = 0;
    unsigned int right = 0;
    left =  (num & (((1 << 5) - 1) << 5));
    left >>= 5; /* shifting to be in the first 5 bits */
    right = (num & ((1 << 5) - 1));
    converted[0] = base32[left];
    converted[1] = base32[right];
	converted[2] = '\0';
}


void decToBin(char *binary, int num)
{
	int i = 9;
	for(;i>=0;--i)
	{
		binary[i] = '0' + (num & 1);
		num = (num >> 1);
	}
	binary[10] = '\0';
}
void create_ob(FILE *fptrOb)
{char str[10]={0};
	int length = START_MEMORY_SIZE;
	int i = 0;
	char *address = (char *)malloc(3* sizeof(char));
	char *encoding = (char *)malloc(3* sizeof(char));
	toBase32(encoding,dc);
	toBase32(address, ic);
	fprintf(fptrOb, "%s\t\t%s\n",address,encoding);

	for (; i < ic;length++, i++)
	{	decToBin(str,instructions[i]);
		toBase32(address,length);
		toBase32(encoding, instructions[i]);
		fprintf(fptrOb,"%s\t\t%s \t\t\t%s\n",address, encoding,str);
memset(str,'\0',10);
	}

	for (i = 0; i < dc;length++, i++)
	{
		toBase32(address, length);
		toBase32(encoding, data[i]);
		fprintf(fptrOb,"%s\t\t%s\n",address, encoding);
	}
	free(address);
	free(encoding);
}

void create_ext(FILE *fptrExt, struct entExtList *node)
{
	char *extEncoding32 = (char *)malloc(3* sizeof(char));
	while(node->next != NULL) 
	{	if(node->entOrExt == EXTERN)
		{
			toBase32(extEncoding32,node->address);
			fprintf(fptrExt,"%s\t\t%s\n",node->name, extEncoding32);
		}  
		node = node->next;
	}
	free(extEncoding32);
}


void create_ent(FILE *fptrEnt, struct symbolTable *table)
{
	struct symbolNode* node = table->head;
	char *entEncoding32 = (char *)malloc(3* sizeof(char));
	while (node != NULL)
	{
		if (node->s_type == ENTRY)
		{	toBase32(entEncoding32, node->s_value);
			fprintf(fptrEnt,"%s\t\t%s\n", node->s_name, entEncoding32);
		}
		node = node->next;
	}
	free(entEncoding32);
}

/*Second pass functions*/

int registerWord(int option, char *register1, char* register2)
{
	int destReg;
	int wordReg = (int)atoi(register1 + 1);
	if (option == SOURCE_IS_REG)
	{
		wordReg <<= BITS_IN_REG_METHOD;
		wordReg = addAre(wordReg, Absolute);
	}
	else
	{
		if (option == DEST_IS_REG)
		{
			wordReg = addAre(wordReg, Absolute);
		}
		else
		{
			wordReg <<= BITS_IN_REG_METHOD;
			wordReg = addAre(wordReg, Absolute);
			destReg = (int)atoi(register2 + 1);
			destReg = addAre(destReg, Absolute);
			wordReg = wordReg | destReg;
		}
	}
	return encodeInstruction(wordReg);
}

int insertLabel(char* label, symbolTable* table, entExtList** e_Head)
{
	int encoding = 0;
	symbolNode* node = table->head;
	while(node != NULL)
	{
		if (!strcmp(node->s_name, label))
		{
			encoding = node->s_value;
			if (node->s_type == EXTERN)
			{	
				insertExtEnt(e_Head, node->s_name, ic+100,EXTERN);
                encoding = addAre(encoding, External);
			}
			else
			{
				encoding = addAre(encoding, Relocatable);
			}
			return encodeInstruction(encoding);
		}
		node = node->next;
	}
	printf("Error in line %d: Command label doesnt exist.\n", lineNum);
	return 0;
}

int addAre(unsigned int areWord, int areType)
{
	areWord <<= 2;
	areWord = areWord | areType;
	return areWord;
}

int encodeInstruction(int word)
{
	if (word <= 1023 && word >= -1023)
	{
		instructions[ic++] = word;
		return 1;
	}
	printf("Error in line %d: The encode of instruction word too bigg , because she over the 10 bits in binary.\n", lineNum);
	return 0;
}

void insertExtEnt(entExtList **e_Head, char *name, int address, int type)
{
    entExtList *new_node = (entExtList*)malloc(sizeof(entExtList));
    strcpy(new_node->name, name);
    new_node->address = address;
    new_node->entOrExt = type; 
    new_node->next = *e_Head;
    *e_Head = new_node;
}

int insertWord(int method, char *operand, symbolTable* head, entExtList **e_Head)
{
	int word;
	char *temp = '\0';
/*(char*)malloc((strlen(operand)+1)*sizeof(char));*/
	if (method == Immediate)
	{
		temp = strtok(operand, "#");
		operand++;
		word = (unsigned int)atoi(operand);
		word = addAre(word, Absolute);
		return encodeInstruction(word);
	}
	else
	{
		if (method == To_struct)
		{
			temp = strtok(operand, ".");
			insertLabel(temp, head, e_Head);
			operand++;
			word = (unsigned int)atoi(operand);
			word = addAre(word, Absolute);
			return encodeInstruction(word);
		}
		else
			return insertLabel(operand, head, e_Head);
	}
	return 0;
}

int firstWord(int commandType, int isSrc, int isDest, int srcMethod, int destMethod)
{
	int firstWord = commandType;
	if (srcMethod == unknownM)
	{
		srcMethod = 0;
	}
	if (destMethod == unknownM)
	{
		destMethod = 0;
	}
	firstWord <<= METHOD_BITS_IN_FIRST_WORD; 
	firstWord |= srcMethod;
	firstWord <<= METHOD_BITS_IN_FIRST_WORD;
	firstWord |= destMethod;
	firstWord = addAre(firstWord, Absolute);
	return encodeInstruction(firstWord);
}

int encodeOperands(char *src, char *dest, int isSrc, int isDest, int srcMethod, int destMethod, symbolTable* head, entExtList **e_Head)
{
	int result = 0;
	if (isSrc)
	{
		if (srcMethod == Register)
		{
			if (isDest && destMethod == Register)
			{
				return registerWord(BOTH_IS_REG, src, dest);
			}
			else
				result = registerWord(SOURCE_IS_REG, src, dest);
		}
		else
			result = insertWord(srcMethod, src, head, e_Head);
	}
	if (isDest)
	{
		if (destMethod == Register)
		{
			result = registerWord(DEST_IS_REG, dest, src);
		}
		else
			result = insertWord(destMethod, dest, head, e_Head);
	}
	return result;
}

/*
Bool doesSymbolExist(symbolTable* table, const char * symbol)
{
	symbolNode * ptr = table->head;
	char tmpWord[16] = {0};
	char * token = NULL;
	while (ptr && symbol)
	{
		memset(tmpWord,'\0',sizeof(tmpWord));
		strcpy(tmpWord,symbol);
		token = strtok(tmpWord,":");
		if (!strcmp(token, ptr->s_name))
			return TRUE;
		ptr= ptr->next;
	}
	return FALSE;
}

int findTheNumberOfOperands(char **arr, symbolTable* table)
{
	int count = 0;
	int numOfOperands = -1;
	int i;
	
	for (i = 0; arr[i] != NULL; ++i)
	{
		if (i == 0 && doesSymbolExist(table, arr[i]))
		{
			continue;
		}

		if (findCommand(arr[i]) != -1)
		{
			count = numOfOperands = getNumOfOpsByCmd(arr[i]);
			continue;
		}

		if (numOfOperands != -1)
		{
			if (strcmp(arr[i], ",") && (strlen(arr[i]) > 0))
				count --;
		}
	}
	
	return (count == 0) ? numOfOperands : -1;
}
*/
int encodeCommandLine(int type, char** arr, int i, symbolTable* table, entExtList **e_Head)
{	
	int isSrc = FALSE, isDest = FALSE, srcMethod = unknownM, destMethod = unknownM;
	int tmp = getNumOfOpsByCmd(arr[i]);
	char *src = '\0';/*אפשרי להקצות עם מאלוק בגודל של השם הכי גדול משיטות המיעון*/
	char *dest = '\0';
	if (tmp != -1)
	{
		if (tmp == 2)
		{
			isSrc = TRUE, isDest = TRUE;
		}
		else
			if (tmp == 1)
			{
				isSrc = FALSE, isDest = TRUE;
			}
			else
			{
				isSrc = FALSE, isDest = FALSE;
				return firstWord(type, isSrc, isDest, srcMethod, destMethod);
			}
	}
	if (isSrc || isDest)
	{
		i++;
		if (isSrc && isDest)
		{
			src = arr[i];
			i = i + 2;
			dest = arr[i];
		}
		else
		{
			dest = arr[i];
			src = NULL;
		}
	}
	if (isSrc)
	{
		srcMethod = methodType(src);
	}
	if (isDest)
	{
		destMethod = methodType(dest);
	}
	firstWord(type, isSrc, isDest, srcMethod, destMethod);
	return encodeOperands(src, dest, isSrc, isDest, srcMethod, destMethod, table, e_Head);
}


/*second pass mainly function*/


void second_pass(FILE *fptrAm, char* sourceName, struct symbolTable *labelList)
{
	int error = FALSE, entry = FALSE, i = 0, cmd = unknownComm;
	int commaCount = 0, quMarks = 0, wordCount = 0;
	char *tmpLine = NULL;
	char **arr = NULL;
	char labelName[MAX_LEN_LABEL+1];
	char strStorage[MAX_LENGTH + 2];
	char line[MAX_LENGTH + 2];
	struct entExtList *node;
	struct entExtList *e_Head =(entExtList*)calloc(1,sizeof(entExtList));
	e_Head->next = NULL;
	ic = 0, lineNum = 0;
	memset(line, '\0', MAX_LENGTH + 2);
	while(fgets(line, MAX_LENGTH + 2, fptrAm))
	{
		i = 0;
		lineNum ++;
		commaCount = isValidLen(line);
		if (commaCount == -2) /*empty line indication*/
			continue;
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
		strcpy(labelName, arr[i]);
		int len = strlen(labelName) - 1;
		if (labelName[len] == ':')/*check symbol before ext or ent label because we need ignore*/
		{
			labelName[strlen(labelName)-1] = '\0';
			if(isExistSym(labelList, labelName, FALSE) || isParamLabel(labelName)){i++;}
			memset(labelName,'\0',MAX_LEN_LABEL+1);
		}
		
		if(labelType(arr[i]) == ENTRY || labelType(arr[i]) == EXTERN)
		{
			if(labelType(arr[i]) == ENTRY)/*יודעים בוודאות שיש אחרי זה מילה אחת בדיוק,לא יותר ולא פחות כי בדקנו תקינות כזו במעבר 1*/
			{
				i++;
				insertExtEnt(&e_Head,arr[i],lineNum,ENTRY);
				/*tmpEnt = (char*)malloc((strlen(arr[i])+1) * sizeof(char));
				strcpy(tmpEnt, arr[i]);
				entryArr_s = (entryArr) ? strlen(entryArr)+ strlen(tmpEnt) +1 : strlen(tmpEnt) + 1;
				entLine_s = (entLine) ? (sizeof(entLine)/sizeof(int)) + 1 : 1;
				entryArr = (char*)realloc(entryArr, entryArr_s*sizeof(char));
 				entLine = (int*)realloc(entLine, entLine_s*sizeof(int));
				strcat(entryArr, tmpEnt);
				strcat(entryArr,SEPARATION_FLAG);
				entLine[entLine_s-1] = lineNum;
				free(tmpEnt);*/

			}
		}
		else
		{
			strcpy(labelName, arr[i]);
			labelName[strlen(labelName)-1] = '\0';
			i = isExistSym(labelList, labelName, FALSE) ? i + 1 : i;
		}
		if (labelType(arr[i]) == IS_COMMAND)
		{
			cmd = findCommand(arr[i]);
			if (!encodeCommandLine(cmd, arr, i, labelList, &e_Head))
			{
				error = TRUE;
			}
		}
		free(tmpLine);
		free(arr);
		memset(strStorage, '\0', MAX_LENGTH + 2);
	}
	node = e_Head;
	updateLabelAddress(labelList);
	while(node != NULL)
	{	
		if(node->entOrExt == ENTRY)
		{ 
			lineNum = node->address;
			if(!isExistSym(labelList, node->name, TRUE))
			{
				error = TRUE;
			}
			else
				entry = TRUE;
		}
		node = node->next;
	}
	if(!error)
	{
		create_files(sourceName, labelList, e_Head, entry);
		printf("The second pass was successful!\n");
	}
	else{
		printf("Errors were found in the second pass, therefore no files will be created: .ext, .ent, .ob\n");}
	freeSymList(labelList);
	freeExtList(e_Head);
	return;
}

void updateLabelAddress(struct symbolTable* labelList)
{
	struct symbolNode *ptr = labelList->head;
	while (ptr != NULL)
	{
		if (ptr->s_value < 100 && ptr->s_type != EXTERN)
		{
			ptr->s_value += 100;
		}
		ptr = ptr->next;
	}
}
