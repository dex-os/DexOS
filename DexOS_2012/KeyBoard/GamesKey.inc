;=========================================================;
; GamesKeyBoard                                12/12/2010 ;
;---------------------------------------------------------;
; Games keyboard driver.                                  ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

 ;----------------------------------------------------;
 ; Games Key board             ;call on keyboard irq. ;
 ;----------------------------------------------------;
GamesKeyBoard:	      
	cld  
	xor   eax,eax			     
	in    AL,60h 
	test  al,10000000b
	jnz   @f
	mov   [WaitKPressed],1
;====================================================;
; ctrl down                                          ;
;====================================================;
@@:
	;cmp   al,128                               ; if below 128, then there is a key
	;jae   nokeyGKboard
	mov   BX,AX	      
	and   BX,007Fh				    ; switch high bit of BX to zero 
	and   AL,80h				    ; check high bit of port value 
	jz    Press	    
Release:					    ; high bit = 1: "release" code 
	mov   [KeyDown+bx],00h			    ; write 00 to "down" array element 
	jmp   DoneGK
Press: 
	mov   [ScanCode2],bl			    ; high bit = 0: "press" code 
	mov   [KeyDown+bx],01h			    ; write 01 to "down" array element
	jmp   DoneGK
DoneGK:
	in    AL,61h				    ; read port 61h, system ctrl port 
	mov   AH,AL				    ; save value to AH 
	or    AL,80h				    ; set top bit to "1" - reset kbd 
	out   61h,AL				    ; write out value to port 
	xchg  AH,AL				    ; put original value back into AL
	out   61h,AL				    ; rewrite original value in AL 
nokeyGKboard:
	ret


 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;

KeyDown 	  rb   128
justincase2	  rb   128
GameKeyBoardOn	  db   0		   
Q		  equ  10h
A		  equ  1eh
