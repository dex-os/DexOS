;---------------------- CUT HERE -------------------------;
; ======================================================  ;
; Simple hello world! demo, for DexOS Ver6 (CLI .dex)     ;
; ======================================================  ;
format binary as 'dex'
use32							  ;
	ORG	0x1A00000				  ; where our program is loaded to
	jmp	start					  ; jump to the start of program.
	db	'DEX6'					  ; We check for this, to make shore it a valid DexOS file.
;=======================================================  ;
; Start of program.                                       ;
;=======================================================  ;
start:							  ;
	mov	ax,18h					  ;                    
	mov	ds,ax					  ;            
	mov	es,ax					  ;           
;=======================================================  ;
; Get calltable address.                                  ;
;=======================================================  ;
	mov	edi,Functions				  ; fill the function table
	mov	al,0					  ; so we have some usefull functions
	mov	ah,0x0a 				  ;
	int	50h					  ;
	mov	esi,Message				  ;
	call	[PrintString_0] 			  ; print string zero ending
	call	[WaitForKeyPress]			  ; wait for keypress
	ret						  ;
							  ;
Message db 'hello from DexOS',10,13			  ; the message string
	db 'Press any key to exit',10,13,0		  ;
include 'Dex.inc'					  ; Dex inc file
;---------------------- CUT HERE -------------------------;