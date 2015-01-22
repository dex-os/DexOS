;============================================================
; FBasic is based on a idea by rCX, for a fasm macro basic.
; The esae of basic, with the power of ASM.
;
; Code input by:
;   Dex
;   rCX
;   Steve
;   TonyMac
;
; This include is for CLI apps (eg: text and mode 13h etc)
;
; NOTE: FBasic_G.inc for GUI app's
;============================================================
format binary as 'dex'
use32
	ORG   0x1A00000
	jmp   start
	db    'DEX6'
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
	mov   edi,Functions			      ; this is the interrupt
	mov   al,0				      ; we use to load the DexFunction.inc
	mov   ah,0x0a				      ; with the address to dex4u functions.
	int   50h

	jmp   SkipQBASIC

;___________CLS (SCREEN 3)____________
CLS	equ   call [Clstext]

;________________COLOR_________________
macro COLOR NewColor
{
	mov   al,NewColor
	call  [TextColor]
}

;_________________END___________________
macro END
{
	mov   ax,0x0003
	call  [RealModeInt10h]
	call  [Clstext]
	ret
}

;_______________LOCATE_________________
macro LOCATE row,col
{
	pushad
	mov   ah,col
	mov   al,row
	call  [SetCursorPos]
	popad
}

;_______________PRINT_________________
macro PRINT String{
	local .Done
	local .a 

		mov esi, .a
		call [PrintString_0]
		jmp .Done 
 
	.a db String,10,13,0
 
	.Done: 
}

;_______________SCREEN_________________
macro SCREEN mode
{
	push eax
	mov ah,0h

	if mode = 0
		mov al,3h
		call  [RealModeInt10h]
	else if mode = 13
		mov al,13h
		call  [RealModeInt10h]
	end if

	pop eax
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

SkipQBASIC: