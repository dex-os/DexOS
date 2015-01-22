format binary as 'dex'                                    ;
use32                                                     ;
	ORG   0x1A00000 				  ; where our program is loaded to
	jmp   start					  ; jump to the start of program.
	db    'DEX6'					  ; We check for this, to make shore it a valid Dex4u file.
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
                                                          ;
	mov     esi,ModID1                                ;
	call    [ModuleFunction]                          ;
	jc      LetsExit                                  ;
	mov     [mod1],eax                                ;
                                                          ;
	mov     ebx,1                                     ;
	call    [mod1]                                    ;
                                                          ;
	mov     ebx,2                                     ;
	call    [mod1]                                    ;
	call    [WaitForKeyPress] 		          ; Wait for key press
LetsExit:                                                 ;
	ret					          ; Exit.
 ;----------------------------------------------------;
 ; calltable include goes here.                       ;
 ;----------------------------------------------------;

mod1  dd 0
mod2  dd 0
ModID1	       db    'DEX4UMOD'                       ; This the ID of the module.
include 'Dex.inc'				      ; Dex inc file
