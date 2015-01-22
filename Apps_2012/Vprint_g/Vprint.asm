;=========================================================;
; Vesa                                         01/04/2012 ;
;---------------------------------------------------------;
; By Dex.                                                 ;
;                                                         ;
; Here is a simple Vesa print demo.                       ;
; for DexOS V 0.06                                        ;
; To assemble use fasm as follows                         ;
; fasm Vprint.asm Vprint.gex                              ;
;=========================================================;
format binary as 'gex'					  ;
use32							  ;
	ORG	0x1A00000				  ;
	jmp	start					  ;
	db	'GEX6'					  ;
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
;=======================================================  ;
; Load vesa info.                                         ;
;=======================================================  ;
	call	[LoadVesaInfo]				  ;
	mov	edi,VESA_Info				  ;
	mov	ecx,193 				  ;
	cld						  ;
	cli						  ;
	rep	movsd					  ;
	sti						  ;
;=======================================================  ;
; get menu buffer address                                 ;
;=======================================================  ;
	mov	ebx,1					  ;
	mov	ecx,1					  ; set buffer address 
	mov	esi,VesaBuffer				  ;
	int	40h					  ;   
;=======================================================  ;
; Set buffer white and send to screen                     ;
;=======================================================  ;
	call	Set_white				  ; fill the off screen buff with color
	call	Vesa_print				  ;
	call	BuffToScreen				  ; write it to screen
	call	[WaitForKeyPress]			  ;
	ret						  ; return to menu
							  ;
;=======================================================  ;
; BuffToScreen.                                           ;
;=======================================================  ;
BuffToScreen:						  ; test for 24bit or 32bit vesa
	cmp	[ModeInfo_BitsPerPixel],24		  ;
	jne	Try32					  ;
	call	BuffToScreen24				  ;
	jmp	wehavedone24				  ;
Try32:							  ;
	cmp	[ModeInfo_BitsPerPixel],32		  ;
	jne	wehavedone24				  ;
	call	BuffToScreen32				  ;
wehavedone24:						  ;
@@:							  ;
	ret						  ;
							  ;
;=======================================================  ;
; BuffToScreen32 (32bpp)                                  ;
;=======================================================  ;
BuffToScreen32: 					  ;
	 pushad 					  ;
	 push	 es					  ;
	 mov	 ax,8h					  ;
	 mov	 es,ax					  ;
	 mov	 edi,[ModeInfo_PhysBasePtr]		  ;
	 mov	 esi,VesaBuffer 			  ;
	 xor	 eax,eax				  ;
	 mov	 ecx,eax				  ;
	 mov	 ax,[ModeInfo_XResolution]		  ;
	 mov	 cx,[ModeInfo_YResolution]		  ;
	 mul	 ecx					  ;
	 mov	 ecx,eax				  ;
	 cld						  ;
	 cli						  ;
	 rep	 movsd					  ;
	 sti						  ;
	 pop	 es					  ;
	 popad						  ;
	 ret						  ;
							  ;
;=======================================================  ;
; BuffToScreen24 (24bpp)                                  ;
;=======================================================  ;
BuffToScreen24: 					  ;
	 pushad 					  ;
	 push	 es					  ;
	 mov	 ax,8h					  ;
	 mov	 es,ax					  ;
	 xor	 eax,eax				  ;
	 mov	 ecx,eax				  ;
	 mov	 ebx,eax				  ;
	 mov	 ax,[ModeInfo_YResolution]		  ;
	 mov	 ebp,eax				  ;
	 lea	 eax,[ebp*2+ebp]			  ;
	 mov	 edi,[ModeInfo_PhysBasePtr]		  ;
	 mov	 esi,VesaBuffer 			  ;
	 cld						  ;
.l1:							  ;
	 mov	 cx,[ModeInfo_XResolution]		  ;
	 shr	 ecx,2					  ;
.l2:							  ;
	 mov	 eax,[esi]				  ;
	 mov	 ebx,[esi+4]				  ;
	 shl	 eax,8					  ;
	 shrd	 eax,ebx,8				  ;
	 stosd						  ;
							  ;
	 mov	 ax,[esi+8]				  ;
	 shr	 ebx,8					  ;
	 shl	 eax,16 				  ;
	 or	 eax,ebx				  ;
	 stosd						  ;
							  ;
	 mov	 bl,[esi+10]				  ;
	 mov	 eax,[esi+12]				  ;
	 shl	 eax,8					  ;
	 mov	 al,bl					  ;
	 stosd						  ;
							  ;
	 add	 esi,16 				  ;
	 loop	 .l2					  ;
							  ;
	 sub	 ebp,1					  ;
	 ja	.l1					  ;
							  ;
	 pop	 es					  ;
	 popad						  ;
	 ret						  ;
							  ;
;=======================================================  ;
; Vesa_print.                                             ;
;=======================================================  ;
Vesa_print:						  ;
	 pushad 					  ;
	 mov	edx,10					  ;
	 shl	edx,16					  ;
	 mov	dx,10					  ;
	 mov	ecx,0x000000cf				  ;
	 mov	eax,2					  ;
	 xor	edi,edi 				  ;
	 mov	esi,String1				  ;
	 mov	ebx,7					  ;
	 int	40h					  ;
							  ;
	 mov	edx,10					  ;
	 shl	edx,16					  ;
	 mov	dx,35					  ;
	 mov	ecx,0x000000cf				  ;
	 mov	eax,1					  ;
	 mov	edi,18					  ;
	 mov	esi,String1				  ;
	 mov	ebx,7					  ;
	 int	40h					  ;
							  ;
	 mov	edx,10					  ;
	 shl	edx,16					  ;
	 mov	dx,55					  ;
	 mov	ecx,0x00ffffff				  ;
	 mov	eax,2					  ;
	 mov	edi,0x00000000				  ;
	 mov	esi,String1				  ;
	 mov	ebx,9					  ;
	 int	40h					  ;
							  ;
	 mov	edx,10					  ;
	 shl	edx,16					  ;
	 mov	dx,75					  ;
	 mov	ecx,0x00ffffff				  ;
	 mov	eax,1					  ;
	 mov	edi,0x00000000				  ;
	 mov	esi,String1				  ;
	 mov	ebx,9					  ;
	 int	40h					  ;
							  ;
	 mov	ecx,1234				  ;
	 mov	edx,10					  ;
	 shl	edx,16					  ;
	 mov	dx,95					  ;
	 mov	eax,4					  ;
	 shl	eax,16					  ;
	 mov	esi,0x00ffffff				  ;
	 mov	edi,0xff000000				  ;
	 mov	ebx,11					  ;
	 int	40h					  ;
							  ;
	 mov	ecx,1234				  ;
	 mov	edx,10					  ;
	 shl	edx,16					  ;
	 mov	dx,110					  ;
	 mov	eax,4					  ;
	 shl	eax,16					  ;
	 mov	ah,1					  ;
	 mov	esi,0x00ffffff				  ;
	 mov	edi,0xff000000				  ;
	 mov	ebx,11					  ;
	 int	40h					  ;
							  ;
	 mov	ecx,1234				  ;
	 mov	edx,10					  ;
	 shl	edx,16					  ;
	 mov	dx,125					  ;
	 mov	eax,11					  ;
	 shl	eax,16					  ;
	 mov	ah,2					  ;
	 mov	esi,0x00ffffff				  ;
	 mov	edi,0xff000000				  ;
	 mov	ebx,11					  ;
	 int	40h					  ;
	 popad						  ;
	 ret						  ;
;=======================================================  ;
; Fill buffer with color (white)                          ;
;=======================================================  ;
Set_white:						  ;
	 pushad 					  ;
	 mov	edi,VesaBuffer				  ;
	 mov	eax,0x00ffffff				  ;
	 mov	ecx,800*600				  ;
	 rep	stosd					  ;
	 popad						  ;
	 ret						  ;
nnnnn rb 800						  ;
jkhgg db 1						  ;
							  ;
String1 db 'I hope this works, len should not see this',0 ;
String2 db 'wwwww   wwwwwww  wwwwwww wwwww    www',0	  ;
;=======================================================  ;
; Data                                                    ;
;=======================================================  ;
include 'Dex.inc'					  ; Here is where we includ our "Dex.inc" file
VesaBuffer:						  ; our screen buffer
