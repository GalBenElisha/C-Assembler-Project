;unfitting operands
add #1,  #4
lea r5 , #1
inc #1
W: mov illegal_label, r1
sub W, !@
lea W, 3
bne B.3
.struct 3, "f

;extra text
clr W, #3,4

; empty label
label:

;use of undefined labels
add A, B

;illegal numbers
mov #23a, r1

;missing numbers
.data

;illegal comma/s
.data ,3

;illegal / missing string
.string "
.struct 5,

;illegal label name
_!@+: hlt

;declaring an already defined label as external
 hello: hlt
 .extern hello

;defining label that already declared as external
.extern G
G: hlt

;b should be discarded
b: .extern c

;missing label after ".entry" \ ".extern"
.extern
.entry

;second stage errors:
;declaring label as entry without definition
.entry blah

;using non defined labels
mov P, Q
