;=========================================================;
; Test module                                    25/08/07 ;
;---------------------------------------------------------;
; By Dex.                                                 ;
;                                                         ;
; Here is a simple Module demo.                           ;
; Assemble like this:                                     ;
; c:\fasm Test.asm Test.obj                               ;
;=========================================================;
format COFF
	jmp   ModRun			 ; jump to the start of program.
    Signature	  db	'MOD1'		 ; We check it's a valid Module file.
    ModID	  db	'DEX4UMOD'	 ; This the ID of the module.
    SizeOfModule  dd	 ModEnd 	 ; This put at end of module.
    ModLoadPtr	  dd	 ModLoad	 ; This is to run code on first load of module.
    ModUnLoadPtr  dd	 ModUnLoad	 ; This is to run code on unload of module.
    ModNumberPtr  dd	 2		 ; This is the number of functions.

ModRun:
	pushad				 ; ************ [STARTUP CODE HERE] ************
	push  ds
	push  es
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


	mov   esi,MsgModLoadOK
	call  [PrintString_0]
	pop   es
	pop   ds
	popad
	ret				; This returns to the CLI/GUI

ModLoad:				; ************ [HERE IS CODE TO CALL FUNCTIONS] ************
	push  ds
	push  es
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	cmp   ebx,[ModNumberPtr]
	ja    ModError
	shl   ebx,2
	add   ebx,ModFunctions
	call  dword[ebx]
	jc    ModError
ModOK:
	pop   es
	pop   ds
	clc
	ret


ModError:
	pop   es
	pop   ds
	stc
	ret


ModUnLoad:				; ************ [UNLOAD MOD CODE HERE] ************
	ret

ServiceUnUsed:				; Unused function.
	ret

Service1:				; ************ [FIST FUNCTON CODE HERE] ************
	pushad
	mov   esi,msgService1
	call  [PrintString_0]
	popad
	ret
Service2:				; ************ [SECOND FUNCTON CODE HERE] ************
	pushad
	mov   esi,msgService2
	call  [PrintString_0]
	popad
	ret

 ;----------------------------------------------------;
 ; Start of code in module.                           ;
 ;----------------------------------------------------;

ModFunctions:
		  dd	 ServiceUnUsed	; Reserve the first one.
		  dd	 Service1	; Points to first function.
		  dd	 Service2	; Points to second function.
 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
MsgModLoadOK db 'Test module loaded',10,13,13,0
msgService1  db 'Hello from Service1!',10,13,0
msgService2  db 'Hello from Service2!',10,13,0
 ;----------------------------------------------------;
 ; BSS goes here.                                     ;
 ;----------------------------------------------------;

align 4 					      ;-----+
Cut  db '2CUT'					      ; These must be here
						      ;-----+
include 'Dex.inc'						       ; Dex inc file
ModEnd: