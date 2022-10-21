
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt

org 100h
.DATA
number DW 7
all  DW 2
temp DB 10

.CODE
MOV SI,10
MOV AX,@DATA
MOV DS,AX   
MOV dx,offset number
MOV ah,0AH
INT 21H
MOV BP,offset number+3
XOR CX,CX
CMP [BP],30H
JG  jump
less:

DEC BP
ADD CX,[bp]
SUB CX,30H
XOR CH,CH   

MOV BP,2
MOV SP,2

FOR:
CMP BP,SP
JE  PRIME
MOV AX , BP
XOR DX,DX
DIV SP
CMP DL,0
JE  PLUS
INC SP
JMP for

PRIME:
MOV DL,0
MOV ah,02H
INT 21H    

XOR DX,DX
MOV AX,BP
DIV SI
MOV BH,AL
MOV BL,DL
CMP BH,0
JE continue
MOV DL,BH
ADD DL,48
MOV AH,02H
INT 21H
continue:
XOR DX,DX
MOV DL,BL
ADD DL,48
MOV ah,02H
INT 21H

dec CX
CMP CX,0
JE  EXIT
MOV SP,2
INC BP
JMP FOR


PLUS:
INC BP
MOV SP,2
JMP FOR
jump:
MOV CX,[bp]
SUB CX,30H
SHL CX,1
JMP less

EXIT:
MOV DX,4CH
INT 21H





