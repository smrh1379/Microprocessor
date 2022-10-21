
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt

org 100h

.DATA
data1  DD ffffffffH
data2  DD ffffffffH
result DQ 8 DUP(0)
.CODE
MOV AX,@DATA
MOV DS,AX 
MOV SI,OFFSET data1
MOV DI,OFFSET data2
MOV BP,OFFSET result
MOV CX,02
BACK: MOV AX,[SI]
MOV BX,[DI]
MUL BX
MOV [BP]  ,AX
MOV [BP]+2,DX
INC SI
INC SI
INC DI
INC DI
INC BP
INC BP
INC BP
INC BP
LOOP BACK

ret




