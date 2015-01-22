;=========================================================;
; FPS TEST                                       05/08/06 ;
;---------------------------------------------------------;
;                                                         ;
; Assemble C:\Fasm FpsTest.ASM FpsTest.dex                ;
;=========================================================;
format binary as 'dex'
	ColorOrder equ Dex4U
use32
	ORG   0x1A00000
	jmp   start
	db    'DEX6'
msg1:	db " Vesa mode not supported",10,13
	db " Press any key to exit. ",10,13,0
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
;======================================================;
; Load vesa info.                                      ;
;======================================================;
	mov   ax,4f00h
	mov   bx,0x4115
	mov   edi,Mode_Info
	call  [RealModeInt10h]
	jc    VesaError1

 ;----------------------------------------------------;
 ; Load vesa info.                                    ;
 ;----------------------------------------------------;

	mov   eax,0x00ffffff
	call  FillScreenColor
	mov   al,0x0
	out   0x70,al
	in    al,0x71
	mov   [lastSecondTick],al
	mov   [FpsCount],0
FpsLoop:
	call  PrintFPS
	call  BuffToScreen
	inc   [FpsCount]
	call  [KeyPressedNoWait]
	cmp   al,0
	je    FpsLoop
ExitHere:
	mov   ax,03h
	call  [RealModeInt10h]
	xor   eax,eax
	call  [SetCursorPos]
	ret

 ;----------------------------------------------------;
 ; Display Vesa Error message.                        ;
 ;----------------------------------------------------;
VesaError1:
	mov   ax,03h
	call  [RealModeInt10h]
	xor   eax,eax
	call  [SetCursorPos]
	mov   esi,msg1
	call  [PrintString_0]
	call  [WaitForKeyPress]
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; FillScreenColor (buffer)                           ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:   eax = color                              ;
 ;                                                    ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
FillScreenColor:
	pushad
	mov  edi,VesaBuffer
	xor  ecx,ecx
	mov  cx,[ModeInfo_YResolution]
FillScreenColorLoop:
	push ecx
	xor  ecx,ecx
	mov  cx,[ModeInfo_XResolution]
	cld
	rep  stosd
	pop  ecx
	loop FillScreenColorLoop
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; FillScreenColorSmall (buffer)                      ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:   eax = color                              ;
 ;                                                    ;
 ;                                                    ;
 ; Output:                                            ;
 ;                                                    ;
 ;....................................................;
FillScreenColorSmall:
	pushad
	mov  edi,VesaBuffer
	xor  ecx,ecx
	mov  cx,20
FillScreenColorLoopS:
	push ecx
	xor  ecx,ecx
	mov  cx,[ModeInfo_XResolution]
	cld
	rep  stosd
	pop  ecx
	loop FillScreenColorLoopS
	popad
	ret

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;

include 'FPS1.inc'
include 'VesaText.inc'
include 'Font16.inc'

lastSecondTick	db 0
FpsCount	dw 0
count		db 0
align 4
include 'Dex.inc'
align 16
VesaBuffer1 rd	800*600*2
align 16
VesaBuffer: