;=========================================================;
; Int10h                                       01/01/2011 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

Int10h_Max = 256

int_10h:						  ; AH = function number
	pushad						  ;
	push	ds					  ;
	push	es					  ;
	push	eax					  ; *
	mov	ax,18h					  ; *
	mov	es,ax					  ; *
	mov	ds,ax					  ; *
	pop	eax					  ; *
	cmp	ax,3h					  ;
	je	Switch2Textmode 			  ;
	cld						  ;
	cmp	ah,4fh					  ; function 4fh:
	je	Int10h_Vesa				  ;  vesa functions
	jmp	Do_int_10h				  ; all other functions 
	mov	word[RealModeAX],0xde			  ; unavailable function code
	jmp	int_errorN				  ;
;=======================================================  ;
; Start of functions                                      ;
;=======================================================  ;
  Switch2Textmode:					  ;
	call	RealModeInt10h				  ;
	cmp	byte [RealModeError],0			  ;
	je	IntBios_okN				  ;
	jmp	int_errorN				  ;
  Do_int_10h:						  ; function ??h:
	call	RealModeInt10h				  ;
	cmp	byte [RealModeError],0			  ;
	je	IntBios_okN				  ;
	jmp	int_errorN				  ;
  Int10h_Vesa:						  ; function 4fh:
	mov	 dword[VesaEDI],edi			  ;  AX = 4F00h
	call	 VesaRmInt				  ;  ES:EDI -> buffer for SuperVGA information
	cmp	 ah,1					  ;  BX = VESA MODE
	je	 int_errorN				  ;
	mov	 esi,Mode_Info				  ;
	mov	 edi,dword[VesaEDI]			  ;
	mov	 ecx,200				  ;
	cld						  ;
	rep	 movsd					  ;
	jmp	 int_okN 				  ;

;=======================================================  ;
;  int_error                                              ;
;=======================================================  ; 
IntBios_okN:						  ;                     
	pop	es					  ;
	pop	ds					  ;
	popad						  ;
	mov	ax,word[RealModeAX]			  ;
	mov	bx,word[RealModeBX]			  ;
	mov	cx,word[RealModeCX]			  ;
	mov	dx,word[RealModeDX]			  ;
	clc						  ;
	ret						  ;

;=======================================================  ;
;  int_error                                              ;
;=======================================================  ; 
int_okN: 						  ;
	pop	es					  ;
	pop	ds					  ;
	popad						  ;
	mov	ax,word[RealModeAX]			  ;
	clc						  ;
	ret						  ;

;=======================================================  ;
;  int_error                                              ;
;=======================================================  ; 
int_errorN:						  ;
	pop	es					  ;
	pop	ds					  ;
	popad						  ;
	mov	ax,word[RealModeAX]			  ;
	stc						  ;
	ret						  ;

;=======================================================  ;
;  data                                                   ;
;=======================================================  ;
error_code     dd     0					  ;

align 4
VesaEDI        dd     0


