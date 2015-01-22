;====================================================================================;
; Mouse                                                                   20/04/2012 ;
;------------------------------------------------------------------------------------;
; By Dex.                                                                            ;
;                                                                                    ;
; Here is a text mode mouse example.                                                 ;
; To assemble use fasm as follows                                                    ;
; c:\fasm mouse.asm mouse.dex                                                        ;
;                                                                                    ;
;====================================================================================;
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
	mov   edi,Functions
	mov   al,0
	mov   ah,0x0a
	int   50h
 ;----------------------------------------------------;
 ; Draw box.                                          ;
 ;----------------------------------------------------;
	call  [Clstext]
	mov   ah,6
	mov   al,8
	call  [SetCursorPos]
	mov   esi,Box
	call  [PrintString_0]
 ;----------------------------------------------------;
 ; Mouse setup.                                       ;
 ;----------------------------------------------------;
	call  [ResetMouse]
	call  [ShowMouse]
	int   6Ch
 ;----------------------------------------------------;
 ; Main Mouse loop.                                   ;
 ;----------------------------------------------------;
MouseLoop:
	cmp   word[X],cx
	je    @f
	mov   word[X],cx
	mov   ah,1
	mov   al,1
	call  [SetCursorPos]
	jc    @f
	mov   esi,mouseX
	call  [PrintString_0]
	mov   ax,cx
	call  [WriteHex16]
@@:
	cmp   word[Y],dx
	je    @f
	mov   word[Y],dx
	mov   ah,3
	mov   al,1
	call  [SetCursorPos]
	jc    @f
	mov   esi,mouseY
	call  [PrintString_0]
	mov   ax,dx
	call  [WriteHex16]
@@:
	call  [GetMousePos]
	shr   cx,3
	shr   dx,3
	test  bl,00000100b
	jz    MouseLoop
	cmp   cx,8
	jb    MouseLoop
	cmp   cx,23
	ja    MouseLoop
	cmp   dx,6
	jb    MouseLoop
	cmp   dx,9
	ja    MouseLoop
 ;----------------------------------------------------;
 ; Stop mouse on exit.                                ;
 ;----------------------------------------------------;
	call  [HideMouse]
	call  [Clstext]
	ret


 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
 mouseX: db 'MouseX = ',0
 mouseY: db 'MouseY = ',0

 Box:	 db 'ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿',10,13
 db '        ³ Click  here  ³',10,13
 db '        ³   to Exit    ³',10,13
 db '        ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ',10,13,0
 X dw 0
 Y dw 0
include 'Dex.inc'
