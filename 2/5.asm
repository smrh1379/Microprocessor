
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt

org 100h
.CODE 
FINAL DW 00h
num db 1
.DATA
MOV AH,0AH
INT 21H
MOV CX,7
MOV SI,CX
J:
PUSH CX
DEC CX
CMP CX,00H
JE  reset
JMP J

find:
POP AX
POP BX
MUL BX
PUSH AX
LOOP find
MOV FINAL,AX   
MOV DX,AX
JMP exit
reset:
MOV CX,SI
DEC CX
JMP find

EXIT:
MOV AH,4CH
INT 21H

ret




