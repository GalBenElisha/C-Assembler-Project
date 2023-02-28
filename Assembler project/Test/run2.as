MAIN: mov S1.1 ,LENGTH 
	 add r2,STR 
LOOP: jmp END
	macro m1 
		 inc K
		 mov S1.2 ,r3
	endmacro 
 	prn #-5 
.entry XYZ
	 sub r1, r4
	m1 
 	bne LOOP 
END: hlt 
STR: .string "abcdef" 
 	LENGTH: .data 6,-9,15 
K: .data 22 

	lea L3.1, LENGTHname
S1: .struct 8, "ab"

Z:		.data 22
MAINTEST:	mov S123.1, D
		add r2,STRing
S2:		.struct -1	 , 	 "12345"
.extern D
XYZ: 	jmp D
		macro m2
		inc Z
; This is comment!!!
	
		mov S123.2 ,	r3	
		endmacro
		prn 	#-50

		sub r1	, r4
		m2
		
		prn Z.2
		bne L3
ENDName:	hlt
STRing:	.string "abcdef"
LENGTHname:	.data 6,-512,511
S123:		.struct 8, "ab"
.entry Z

.extern L3

mov r0 , r6


