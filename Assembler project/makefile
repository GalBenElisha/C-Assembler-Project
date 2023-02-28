assembler: main.o firstPass.o secondPass.o macro.o utils.o
	gcc -g -ansi -Wall -pedantic main.o firstPass.o secondPass.o macro.o utils.o -o assembler

main.o: main.c secondPass.h
	gcc -c -g -ansi -Wall -pedantic main.c -o main.o

firstPass.o: firstPass.c firstPass.h
	gcc -c -g -ansi -Wall -pedantic firstPass.c -o firstPass.o

secondPass.o: secondPass.c secondPass.h
	gcc -c -g -ansi -Wall -pedantic secondPass.c -o secondPass.o

macro.o: macro.c macro.h
	gcc -c -g -ansi -Wall -pedantic macro.c -o macro.o

utils.o: utils.c utils.h
	gcc -c -g -ansi -Wall -pedantic utils.c -o utils.o


