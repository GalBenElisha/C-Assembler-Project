#ifndef MY_HEADER_FILE_H
#define MY_HEADER_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>


#define SPACE " \r\t\n"
#define WHITE_SPACE " \t\r"
#define FALSE 0
#define TRUE 1
#define MAX_STRUCT_PARAMS 2
#define ENTRY 1
#define EXTERN 2
#define IS_DIRECTIVE 0
#define IS_COMMAND 3
#define PASS1 1
#define PASS2 2
#define MAX_REG_NAME 2
#define METHOD_BITS_IN_FIRST_WORD 2
#define BITS_IN_REG_METHOD 4
#define SOURCE_IS_REG 1 
#define DEST_IS_REG 2
#define BOTH_IS_REG 3
#define MAX_LEN_LABEL 30
#define MAX_LEN_MACRO 30
#define MAX_LENGTH 80
#define START_MEMORY_SIZE 100
#define IS_BIN 2
#define IS_DEC 1
#define MAX_MEMORY_SIZE 2000
#define MAX_MACRO_LEN 31
#define SEPARATION_FLAG " "


enum commands { mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, get, prn, jsr, rts, hlt, unknownComm = -1 };

enum address_types { Immediate, Direct, To_struct, Register, unknownM = -1 };

enum A_R_E { Absolute, External, Relocatable };

enum registers { r0, r1, r2, r3, r4, r5, r6, r7, None = -1 };

enum filesType { am, ob, ent, ext };

#endif
