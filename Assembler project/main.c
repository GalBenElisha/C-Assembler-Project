#include "SecondPass.h"

int main(int argc, char *argv[])
{
	int i;
	char *filename = NULL;
	FILE* fptrAs = NULL;
	FILE* fptrAm = NULL;

	struct symbolTable * list  = NULL;
	printf("***Assembler program***\n\n");
	for (i=1; i < argc; i++)
	{
		filename  = file_to_as(argv[i]);
		fptrAs = fopen(filename, "r");
		if(fptrAs == NULL)
		{
			printf("***CAN NOT OPEN FILE !***\n");
			free(filename);
			return 0;
		}
		else
		{
			filename = file_to_type(argv[i], am);
			fptrAm = fopen(filename, "w");
			
			if (!preAssembler(fptrAs, fptrAm))
			{
				fclose(fptrAm);
				fclose(fptrAs);
               			remove(filename); /*delete this file */
			}
			
			fflush(fptrAs);
			fclose(fptrAm);
			fptrAm = fopen(filename, "r");
			list = firstPass(fptrAm);

			if (list)
			{
				rewind(fptrAm);
				second_pass(fptrAm, argv[i], list);
			}
			fclose(fptrAm);
			printf("***ASSEMBLING PROCESS FINISHED ON FILE : %s ***\n\n", filename);
			
		}
		free(filename);
		memset(data,0,MAX_MEMORY_SIZE * sizeof(int));
		memset(instructions,0,MAX_MEMORY_SIZE * sizeof(int));
	}
	return 0;
}
