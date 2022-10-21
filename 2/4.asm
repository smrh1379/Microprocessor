
; You may customize this and other start-up templates; 
; The location of this template is c:\emu8086\inc\0_com_template.txt

org 100h

.DATA

array dw 15,12,31,15,211,13,51,236,1185,12,5,1,85,1,76,94
temp dw 2
ans dw 0



.CODE      
MOV SI,OFFSET array  
MOV CX,16
MOV BX,10
FOR:
MOV AX,[SI]  
XOR dx,dx
DIV BX
MOV BP,Ax
MOV SP,Dx
XOR DX,DX
MOV AX,SP
DIV TEMP
CMP DL,1
JE  SUM
JMP ODD
SUM:
ADD DI,SP
ODD:
CMP BP,0
JE  reset
XOR dx,dx
MOV AX,BP
DIV BX
MOV BP,Ax
MOV SP,Dx
XOR DX,DX
MOV AX,SP
DIV TEMP
CMP DL,1
JE  SUM 
JMP ODD
reset:
INC SI
INC SI
DEC CX
CMP CX,0
JE EXIT
JMP FOR

EXIT:   
XOR DX,DX
MOV AX,DI
MOV SI,10
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


