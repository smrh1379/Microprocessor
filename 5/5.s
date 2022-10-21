
	EXPORT SystemInit
    EXPORT __main


	AREA mycode,CODE,READONLY 

SystemInit FUNCTION
	; initialization code
 ENDFUNC


; add numbers from 1 to 9 
; main logic of code
__main FUNCTION
	ENTRY
		LDR R0,=0X5CBCFF9;0XAAAAAAAA;0X5CBCFF9;2_10101;
		STR R0,[SP]
		MOV R2,#0;TEDADE 101
		MOV R5,#0;TEDADE SHIFT

		LDR R4,[SP]
SET		LSR R4,R4,#1
		ADD R5,R5,#1
		CMP R4,#0
		BNE SET
		SUB R5,#3
		
FOR
		LDR R1,[SP]
		LSR R1,R1,R5
		AND R1,#7
		EOR R1,#2_101
		CMP R1,#0
		ADDEQ R2,1
		SUB R5,R5,#1
		CMP R5,#-1
		BNE FOR
		
OUT
	B OUT

 ENDFUNC	
 END