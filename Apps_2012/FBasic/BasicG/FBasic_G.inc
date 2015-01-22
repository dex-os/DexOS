;============================================================
; FBasic is based on a idea by rCX, for a fasm macro basic.
; The esae of basic, with the power of ASM.
;
; Code input by:
;   Dex
;   rCX
;   Stephen
;   TonyMac
;
; This include is for GUI apps (eg: high res vesa modes etc)
;
; NOTE: FBasic_C.inc for CLI app's
;============================================================
format binary as 'gex'

use32
	ORG   0x1A00000
	jmp   start
	db    'GEX6'
	include 'BFunction\G_functions.inc'
 ;----------------------------------------------------;
 ; Start of program.                                  ;
 ;----------------------------------------------------;
start:
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
 ;----------------------------------------------------;
 ; Get calltable address.                             ;
 ;----------------------------------------------------;
	mov   edi,Functions
	mov   al,0
	mov   ah,0x0a
	int   50h
 ;----------------------------------------------------;
 ; get menu buffer address                            ;
 ;----------------------------------------------------;

	mov    ebx,1
	mov    ecx,0
	int    40h
	mov    dword[MainMenuBuffer],esi
 ;----------------------------------------------------;
 ; Move the background image to our buffer            ;
 ;----------------------------------------------------;
	call   MoveBuffer
 ;----------------------------------------------------;
 ; get menu buffer address                            ;
 ;----------------------------------------------------;
	mov   ebx,1
	mov   ecx,1
	mov   esi,VesaBuffer
	int   40h

	jmp   SkipQBASIC
;________________SPRITE________________
macro SPRITE   SpriteX, SpriteY, SpriteAddress
{
	pushad
	mov	edi,VesaBuffer
	mov	esi,SpriteAddress
	mov	eax,SpriteX
	mov	ecx,SpriteY
	mov	ebx,6
	int	40h
	popad
}
;______________LOAD_IMAGE_______________
macro LOAD_IMAGE ImageName, ImageBuffer
{
	pushad
	mov	esi,ImageName
	mov	edi,ImageBuffer
	mov	ecx,VesaBufferTemp
	mov	ebx,3
	int	40h
	;jc      ExitError
	popad

}
;______________FONT_SIZE_______________
macro FONT_SIZE DeFaultFontSize
{
	local .NoGood
	local .OK
	push  eax
	mov   eax,DeFaultFontSize
	cmp   eax,1
	je    .OK
	cmp   eax,2
	jne   .NoGood

.OK:
	mov   dword[DfontSize],eax
.NoGood:
	pop   eax
}
;_______________LOCATE_________________
macro LOCATE row,col
{
	pushad
	mov   ax,row
	mov   bx,col
	mov   word[ScrX],ax
	mov   word[ScrY],bx
	popad
}
;________________COLOR_________________
macro COLOR NewColor
{
	push  eax
	mov   eax,NewColor
	mov   dword[NewTextColor],eax
	pop   eax
}
;_______________PRINT_________________
macro PRINT String{
	local .Done1
	local .Done2
	local .a 

	 pushad
	 mov	dx,[ScrX]
	 shl	edx,16
	 mov	dx,[ScrY]
	 mov	ecx,dword[NewTextColor]
	 mov	eax,dword[DfontSize]
	 xor	edi,edi
	 mov	esi,.a
	 mov	ebx,7
	 int	40h
	 popad
	 cmp	dword[DfontSize],1
	 je	.Done1
	 add	[ScrY],15
	 cmp	[ScrY],600-20
	 jbe	.Done2
	 mov	[ScrY],600-20
	 jmp	.Done2
	.Done1:
	 add	[ScrY],12
	 cmp	[ScrY],600-15
	 jbe	.Done2
	 mov	[ScrY],600-15
	 jmp	.Done2
	.a db String,0
 
	.Done2:
}
;_____________CLS (color)______________
Macro CLS   _color1
{
	 pushad
	 mov	edi,VesaBuffer
	 mov	eax,_color1
	 mov	ecx,800*600
	 rep	stosd
	 popad
}
;________________GOTO__________________
Macro GOTO _op1
{
	jmp _op1
}

;________________GOSUB_________________
Macro GOSUB _subname
{
	call _subname
}

;________________RETURN________________
Macro RETURN
{
	ret
}

;________________SLEEP_________________
macro SLEEP
{
;Output:
;       ah = BIOS scancode of key pressed
;       al = ASCII character of key pressed
	call  [WaitForKeyPress]
}
;_________________END__________________
macro END
{
	ret
}
;_______________SCREEN_________________
macro SCREEN scrX, scrY, scrRGB 	;These will be used as soon as we decide on graphic basic to use.
{
	 pushad
	 call	[LoadVesaInfo]
	 mov	edi,VESA_Info
	 mov	ecx,193
	 cld
	 rep	movsd
	 popad
}
;________________SYNC__________________
macro SYNC
{
	call BuffToScreen
}
MainMenuBuffer	 dd 0
NewTextColor	 dd 0
ScrX		 dw 0
ScrY		 dw 0
DfontSize	 dd 1
SkipQBASIC:
