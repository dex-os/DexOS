format binary as 'dex'
use32
	ORG   0x1A00000 				 ; where our program is loaded to
	jmp   start					; jump to the start of program.
	db    'DEX6'					; We check for this, to make sure it a valid DexOS file.

	DSpc	  equ 42
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
 ;----------------------------------------------------;
 ; Call realmode function for mode change.            ;
 ;----------------------------------------------------;
	mov   ax,0x0013
	call  [RealModeInt10h]
	mov   ax,8h
	mov   es,ax
 ;----------------------------------------------------;
 ; Fill the screen black color.                       ;
 ;----------------------------------------------------;
	mov   edi,0xA0000
	mov   ecx,32000
	mov   ax,0x0000
	rep   stosw
 ;----------------------------------------------------;
 ; Draw the clocks :  :                               ;
 ;----------------------------------------------------;
	mov   edi,0xA0000
	add   edi,13860
	add   edi,320*42
	call  FColon

	mov   edi,0xA0000
	add   edi,13970
	add   edi,320*42
	call  FColon

	mov   edi,0xA0000
	add   edi,20900
	add   edi,320*42
	call  FColon

	mov   edi,0xA0000
	add   edi,21010
	add   edi,320*42
	call  FColon
 ;----------------------------------------------------;
 ; Main loop.                                         ;
 ;----------------------------------------------------;
TimeLoop:
	hlt					       ; This make it print time 18 times a second.
	call  GetTimeF				       ; Get time
	mov   edi,9927				       ; print hour
	add   edi,0xA0000
	add   edi,320*42
	mov   al,[hour]
	call  DoHour
	mov   edi,10037 			       ; print minutes
	add   edi,0xA0000
	add   edi,320*42
	mov   al,[minute]
	call  DoHour
	mov   edi,10146 			       ; print seconds
	add   edi,0xA0000
	add   edi,320*42
	mov   al,[second]
	call  DoHour
	call  [KeyPressedNoWait]		       ; check for keypress, if not loop.
	cmp   al,0
	je    TimeLoop
	mov   ax,18h
	mov   es,ax
 ;----------------------------------------------------;
 ; Exit Clock.                                        ;
 ;----------------------------------------------------;
	mov   ax,0x0003
	call  [RealModeInt10h]
	call  [Clstext]
	ret					      ; Exit.

 ;----------------------------------------------------;
 ; Get Time funtion.                                  ;
 ;----------------------------------------------------;
GetTimeF:
	push  es
	pushad
	call  [GetTime]
	mov   [minute],ah
	mov   [second],al
	shr   eax,16
	mov   [hour],al
	call  Bcd2Bin
	mov   [hour],al
	mov   al,[second]
	call  Bcd2Bin
	mov   [second],al
	mov   al,[minute]
	call  Bcd2Bin
	mov   [minute],al
	popad
	pop   es
	ret

 ;----------------------------------------------------;
 ; DoVLine.                                           ;
 ;----------------------------------------------------;
DoVLine:
	pushad
	mov   esi,VLine
	mov   ecx,25
VLoop1: push  ecx
	mov   ecx,10
VLoop2: lodsb
	or    al,al
	jz    VNoDis
	stosb
	dec   edi
VNoDis: inc   edi
	loop  VLoop2
	add   edi,310
	pop   ecx
	loop  VLoop1
	popad
	ret

 ;----------------------------------------------------;
 ; DoRVLine.                                          ;
 ;----------------------------------------------------;
DoRVLine:
	pushad
	mov  esi,VLine
	mov  ecx,25
VLoop1r:
	push ecx
	mov  ecx,10
VLoop2r:
	lodsb
	or   al,al
	jz   VNoDisr
	mov  ax,[BackGa]
	stosb
	dec  edi
VNoDisr:
	inc  edi
	loop VLoop2r
	add  edi,310
	pop  ecx
	loop VLoop1r
	popad
	ret

 ;----------------------------------------------------;
 ; DoHLine.                                           ;
 ;----------------------------------------------------;
DoHLine:
	pushad
	mov   esi,HLine
	mov   ecx,07
HLoop1:
	push  ecx
	mov   ecx,30
HLoop2:
	lodsb
	or    al,al
	jz    HNoDis
	stosb
	dec   edi
HNoDis:
	inc   edi
	loop  HLoop2
	add   edi,290
	pop   ecx
	loop  HLoop1
	popad
	ret

 ;----------------------------------------------------;
 ; DoRHLine.                                          ;
 ;----------------------------------------------------;
DoRHLine:
	pushad
	mov   esi,HLine
	mov   ecx,07
HLoop1r:
	push  ecx
	mov   ecx,30
HLoop2r:
	lodsb
	or    al,al
	jz    HNoDisr
	mov   ax,[BackGa]
	stosb
	dec   edi
HNoDisr:
	inc   edi
	loop  HLoop2r
	add   edi,290
	pop   ecx
	loop  HLoop1r
	popad
	ret

 ;----------------------------------------------------;
 ; DoHour.                                            ;
 ;----------------------------------------------------;
DoHour:
	or   al,al
	jnz  Not0
	call Do0
	add  edi,DSpc
	call Do0
	jmp  HDone
Not0:	cmp  al,01
	jne  Not1
	call Do0
	add  edi,DSpc
	call Do1
	jmp  HDone
Not1:	cmp  al,02
	jne  Not2
	call Do0
	add  edi,DSpc
	call Do2
	jmp  HDone
Not2:	cmp  al,03
	jne  Not3
	call Do0
	add  edi,DSpc
	call Do3
	jmp  HDone
Not3:	cmp  al,04
	jne  Not4
	call Do0
	add  edi,DSpc
	call Do4
	jmp  HDone
Not4:	cmp  al,05
	jne  Not5
	call Do0
	add  edi,DSpc
	call Do5
	jmp  HDone
Not5:	cmp  al,06
	jne  Not6
	call Do0
	add  edi,DSpc
	call Do6
	jmp  HDone
Not6:	cmp  al,07
	jne  Not7
	call Do0
	add  edi,DSpc
	call Do7
	jmp  HDone
Not7:	cmp  al,08
	jne  Not8
	call Do0
	add  edi,DSpc
	call Do8
	jmp  HDone
Not8:	cmp  al,09
	jne  Not9
	call Do0
	add  edi,DSpc
	call Do9
	jmp  HDone
Not9:	cmp  al,0x0a ;10
	jne  Not10
	call Do1
	add  edi,DSpc
	call Do0
	jmp  HDone
Not10:	cmp  al,0x0b;11
	jne  Not11
	call Do1
	add  edi,DSpc
	call Do1
	jmp  HDone
Not11:	cmp  al,0x0c ;12
	jne  Not12
	call Do1
	add  edi,DSpc
	call Do2
	jmp  HDone
Not12:	cmp  al,0x0d ;13
	jne  Not13
	call Do1
	add  edi,DSpc
	call Do3
	jmp  HDone
Not13:	cmp  al,0x0e ;14
	jne  Not14
	call Do1
	add  edi,DSpc
	call Do4
	jmp  HDone
Not14:	cmp  al,0x0f ;15
	jne  Not15
	call Do1
	add  edi,DSpc
	call Do5
	jmp  HDone
Not15:	cmp  al,0x10 ;16
	jne  Not16
	call Do1
	add  edi,DSpc
	call Do6
	jmp  HDone
Not16:	cmp  al,0x11;17
	jne  Not17
	call Do1
	add  edi,DSpc
	call Do7
	jmp  HDone
Not17:	cmp  al,0x12;18
	jne  Not18
	call Do1
	add  edi,DSpc
	call Do8
	jmp  HDone
Not18:	cmp  al,0x13;19
	jne  Not19
	call Do1
	add  edi,DSpc
	call Do9
	jmp  HDone
Not19:	cmp  al,0x14 ;20
	jne  Not20
	call Do2
	add  edi,DSpc
	call Do0
	jmp  HDone
Not20:	cmp  al,0x15 ;21
	jne  Not21
	call Do2
	add  edi,DSpc
	call Do1
	jmp  HDone
Not21:	cmp  al,0x16 ;22
	jne  Not22
	call Do2
	add  edi,DSpc
	call Do2
	jmp  HDone
Not22:	cmp  al,0x17 ;23
	jne  Not23
	call Do2
	add  edi,DSpc
	call Do3
	jmp  HDone
Not23:	cmp  al,0x18 ;24
	jne  Not24
	call Do2
	add  edi,DSpc
	call Do4
	jmp  HDone
Not24:	cmp  al,0x19 ;25
	jne  Not25
	call Do2
	add  edi,DSpc
	call Do5
	jmp  HDone
Not25:	cmp  al,0x1a ;26
	jne  Not26
	call Do2
	add  edi,DSpc
	call Do6
	jmp  HDone
Not26:	cmp  al,0x1b ;27
	jne  Not27
	call Do2
	add  edi,DSpc
	call Do7
	jmp  HDone
Not27:	cmp  al,0x1c ;28
	jne  Not28
	call Do2
	add  edi,DSpc
	call Do8
	jmp  HDone
Not28:	cmp  al,0x1d ;29
	jne  Not29
	call Do2
	add  edi,DSpc
	call Do9
	jmp  HDone
Not29:	cmp  al,0x1e ;30
	jne  Not30
	call Do3
	add  edi,DSpc
	call Do0
	jmp  HDone
Not30:	cmp  al,0x1f ;31
	jne  Not31
	   call Do3
	   add	edi,DSpc
	   call Do1
	   jmp	HDone
Not31:	   cmp	al,0x20 ;32
	   jne	Not32
	   call Do3
	   add	edi,DSpc
	   call Do2
	   jmp	HDone
Not32:	   cmp	al,0x21 ;33
	   jne	Not33
	   call Do3
	   add	edi,DSpc
	   call Do3
	   jmp	HDone
Not33:	   cmp	al,0x22 ;34
	   jne	Not34
	   call Do3
	   add	edi,DSpc
	   call Do4
	   jmp	HDone
Not34:	   cmp	al,0x23 ;35
	   jne	Not35
	   call Do3
	   add	edi,DSpc
	   call Do5
	   jmp	HDone
Not35:	   cmp	al,0x24 ;36
	   jne	Not36
	   call Do3
	   add	edi,DSpc
	   call Do6
	   jmp	HDone
Not36:	   cmp	al,0x25 ;37
	   jne	Not37
	   call Do3
	   add	edi,DSpc
	   call Do7
	   jmp	HDone
Not37:	   cmp	al,0x26 ;38
	   jne	Not38
	   call Do3
	   add	edi,DSpc
	   call Do8
	   jmp	HDone
Not38:	   cmp	al,0x27 ;39
	   jne	Not39
	   call Do3
	   add	edi,DSpc
	   call Do9
	   jmp	HDone
Not39:	   cmp	al,0x28 ;40
	   jne	Not40
	   call Do4
	   add	edi,DSpc
	   call Do0
	   jmp	HDone
Not40:	   cmp	al,0x29 ;41
	   jne	Not41
	   call Do4
	   add	edi,DSpc
	   call Do1
	   jmp	HDone
Not41:	   cmp	al,0x2a ;42
	   jne	Not42
	   call Do4
	   add	edi,DSpc
	   call Do2
	   jmp	HDone
Not42:	   cmp	al,0x2b ;43
	   jne	Not43
	   call Do4
	   add	edi,DSpc
	   call Do3
	   jmp	HDone
Not43:	   cmp	al,0x2c ;44
	   jne	Not44
	   call Do4
	   add	edi,DSpc
	   call Do4
	   jmp	HDone
Not44:	   cmp	al,0x2d ;45
	   jne	Not45
	   call Do4
	   add	edi,DSpc
	   call Do5
	   jmp	HDone
Not45:	   cmp	al,0x2e ;46
	   jne	Not46
	   call Do4
	   add	edi,DSpc
	   call Do6
	   jmp	HDone
Not46:	   cmp	al,0x2f ;47
	   jne	Not47
	   call Do4
	   add	edi,DSpc
	   call Do7
	   jmp	HDone
Not47:	   cmp	al,0x30 ;48
	   jne	Not48
	   call Do4
	   add	edi,DSpc
	   call Do8
	   jmp	HDone
Not48:	   cmp	al,0x31 ;49
	   jne	Not49
	   call Do4
	   add	edi,DSpc
	   call Do9
	   jmp	HDone
Not49:	   cmp	al,0x32 ;50
	   jne	Not50
	   call Do5
	   add	edi,DSpc
	   call Do0
	   jmp	HDone
Not50:	   cmp	al,0x33 ;51
	   jne	Not51
	   call Do5
	   add	edi,DSpc
	   call Do1
	   jmp	HDone
Not51:	   cmp	al,0x34 ;52
	   jne	Not52
	   call Do5
	   add	edi,DSpc
	   call Do2
	   jmp	HDone
Not52:	   cmp	al,0x35 ;53
	   jne	Not53
	   call Do5
	   add	edi,DSpc
	   call Do3
	   jmp	HDone
Not53:	   cmp	al,0x36 ;54
	   jne	Not54
	   call Do5
	   add	edi,DSpc
	   call Do4
	   jmp	HDone
Not54:	   cmp	al,0x37 ;55
	   jne	Not55
	   call Do5
	   add	edi,DSpc
	   call Do5
	   jmp	HDone
Not55:	   cmp	al,0x38  ;56
	   jne	Not56
	   call Do5
	   add	edi,DSpc
	   call Do6
	   jmp	HDone
Not56:	   cmp	al,0x39  ;57
	   jne	Not57
	   call Do5
	   add	edi,DSpc
	   call Do7
	   jmp	HDone
Not57:	   cmp	al,0x3a  ;58
	   jne	Not58
	   call Do5
	   add	edi,DSpc
	   call Do8
	   jmp	HDone
Not58:	   cmp	al,0x3b  ;59
	   jne	HDone
	   call Do5
	   add	edi,DSpc
	   call Do9
HDone:

	   ret

;*****
;----------------------
FColon:
	   pushad
	   mov	esi,FCln
	   mov	ecx,07
FLoop1:    push ecx
	   mov	ecx,07
FLoop2:    lodsb
	   or	al,al
	   jz	FNoDis
	   stosb
	   dec	edi
FNoDis:    inc	edi
	   loop FLoop2
	   add	edi,313
	   pop	ecx
	   loop FLoop1
	   popad
	   ret
;----------------------
Do0:
	   pushad
	   call DoVLine 		; 1
	   push edi
	   add	edi,8000
	   call DoVLine 		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoRHLine		; 4
	   add	edi,8000
	   call DoHLine 		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do1:
	   pushad
	   call DoRVLine		; 1
	   push edi
	   add	edi,8000
	   call DoRVLine		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoRHLine		; 2
	   add	edi,8000
	   call DoRHLine		; 4
	   add	edi,8000
	   call DoRHLine		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do2:
	   pushad
	   call DoRVLine		; 1
	   push edi
	   add	edi,8000
	   call DoVLine 		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoHLine 		; 4
	   add	edi,8000
	   call DoHLine 		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoRVLine		; 5
	   popad
	   ret


Do3:
	   pushad
	   call DoRVLine		; 1
	   push edi
	   add	edi,8000
	   call DoRVLine		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoHLine 		; 4
	   add	edi,8000
	   call DoHLine 		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do4:
	   pushad
	   call DoVLine 		; 1
	   push edi
	   add	edi,8000
	   call DoRVLine		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoRHLine		; 2
	   add	edi,8000
	   call DoHLine 		; 4
	   add	edi,8000
	   call DoRHLine		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do5:
	   pushad
	   call DoVLine 		; 1
	   push edi
	   add	edi,8000
	   call DoRVLine		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoHLine 		; 4
	   add	edi,8000
	   call DoHLine 		; 6
	   pop	edi
	   add	edi,29
	   call DoRVLine		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do6:
	   pushad
	   call DoVLine 		; 1
	   push edi
	   add	edi,8000
	   call DoVLine 		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoHLine 		; 4
	   add	edi,8000
	   call DoHLine 		; 6
	   pop	edi
	   add	edi,29
	   call DoRVLine		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do7:
	   pushad
	   call DoRVLine		; 1
	   push edi
	   add	edi,8000
	   call DoRVLine		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoRHLine		; 4
	   add	edi,8000
	   call DoRHLine		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do8:
	   pushad
	   call DoVLine 		; 1
	   push edi
	   add	edi,8000
	   call DoVLine 		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoHLine 		; 4
	   add	edi,8000
	   call DoHLine 		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret


Do9:
	   pushad
	   call DoVLine 		; 1
	   push edi
	   add	edi,8000
	   call DoRVLine		; 7
	   pop	edi
	   inc	edi
	   inc	edi
	   push edi
	   sub	edi,957
	   call DoHLine 		; 2
	   add	edi,8000
	   call DoHLine 		; 4
	   add	edi,8000
	   call DoHLine 		; 6
	   pop	edi
	   add	edi,29
	   call DoVLine 		; 3
	   add	edi,8000
	   call DoVLine 		; 5
	   popad
	   ret

;====================================================;
; Bcd2Bin.                                           ;
;====================================================;
Bcd2Bin:
	push  bx
	mov   bl,0x0a
	mov   bh,al
	shr   al,4
	mul   bl
	and   bh,0x0f
	add   al,bh
	pop   bx
	ret

 ;====================================================;
 ; Data.                                              ;
 ;====================================================;

hour:	   db 0
minute:    db 0
second:    db 0
BackG	  dw  0707h
BackGa	  dw  0019
VLine	  db  00,00,00,00,10,00,00,00,00,00
	  db  00,00,00,10,02,08,00,00,00,00
	  db  00,00,10,02,02,02,08,00,00,00
	  db  00,10,02,02,02,02,02,08,00,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,08,00
	  db  00,08,02,02,02,02,02,08,00,00
	  db  00,00,08,02,02,02,08,00,00,00
	  db  00,00,00,08,02,08,00,00,00,00
	  db  00,00,00,00,08,00,00,00,00,00
HLine	  db  00,00,00,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,00,00,00
	  db  00,00,10,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,08,00,00
	  db  00,10,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,08,00
	  db  10,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,08
	  db  00,08,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,08,00
	  db  00,00,08,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,02,08,00,00
	  db  00,00,00,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,08,00,00,00
FCln	  db  00,00,00,10,00,00,00
	  db  00,00,10,02,08,00,00
	  db  00,10,02,02,02,08,00
	  db  10,02,02,02,02,02,08
	  db  00,10,02,02,02,08,00
	  db  00,00,10,02,08,00,00
	  db  00,00,00,08,00,00,00


;--------------------------

 ;----------------------------------------------------;
 ; include goes here.                                 ;
 ;----------------------------------------------------;

include 'Dex.inc'				      ; Dex inc file
