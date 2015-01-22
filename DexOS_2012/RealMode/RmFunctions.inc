;=========================================================;
; RmFunctions                                  01/05/2011 ;
;---------------------------------------------------------;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;

BootNumber1 db 0					  ;
;=======================================================  ;
; Test4dDos.                                              ;
;=======================================================  ;  
Test4dDos:						  ;
	push	ax					  ;
	cmp	[BootProTest],0xff			  ;
	je	NoDosFound				  ;
	mov	ah,19h					  ;
	int	21h					  ;
	mov	[BootNumber],al 			  ;
	add	al,41h					  ;
	mov	[BootNumber1],al			  ;
	mov	[DosLoaded],1				  ;
NoDosFound:						  ;
	pop	ax					  ;
	ret						  ;

;=======================================================  ;
; Get Ram Size.   ;conventional memory size (in KBs)      ;
;=======================================================  ;  
ConvMemory:						  ;
	int	12h					  ;
	movzx	eax, ax 				  ;
	mov	dword[ConvMemorySize],eax		  ;        
	ret						  ;

;=======================================================  ;
; Get Ram Size.                                           ;
;=======================================================  ;  
GetRamSize:						  ;
	mov	ax,0xe801				  ;       
	int	15h					  ;
	jc	@f					  ;
	movzx	ebx,bx					  ;
	shl	ebx,6					  ;             
	movzx	eax,ax					  ;
	add	ebx,eax 				  ;
	mov	[ExtMemorySize],ebx			  ;
	call	TotalRam				  ;
@@:							  ;
	ret						  ;

;=======================================================  ;
; TotalRam.                                               ;
;=======================================================  ;  
TotalRam:						  ;                        
	shr	ebx,10					  ;
	inc	ebx					  ;
	mov	[TotalMemoryMB],ebx			  ;
	ret						  ;
@@:	inc	ebx					  ;
	mov	[TotalMemoryMB],ebx			  ;
	ret						  ;

;=======================================================  ;
; DetectPcIBus                                            ;
;=======================================================  ; 
DetectPcIBus:						  ;
	mov	ax,0xb101				  ;                   
	int	1ah					  ;            
	jc	NoPci					  ;
	cmp	edx,0x20494350				  ;              
	jne	NoPci					  ;
	mov	byte[PciBusPresent],1			  ;
NoPci:							  ;
	ret						  ;

;=======================================================  ;
; ReadPalette.                                            ;
;=======================================================  ; 
ReadPalette:                                              ;
	pusha                                             ;
	mov     dx,StPalette                              ;
	mov     ah,10h                                    ;
	mov     al,9                                      ;
	xor     bx,bx                                     ;
	int     10h                                       ;
	popa                                              ;
	ret                                               ;

;=======================================================  ;
;  Data.                                                  ;
;=======================================================  
StPalette rb 20
