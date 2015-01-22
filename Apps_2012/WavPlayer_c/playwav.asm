;  NBASM playwav ( http://www.frontiernet.net/~fys/newbasic.htm )
;  Converted to FASM and DexOS by Dex
PgPort	   equ 83h
AddPort    equ 02h
LenPort    equ 03h
ModeReg    equ 49h
Channel    equ 01h
BasePort   equ 220h
Freq	   equ 11000

format binary as 'dex'
use32
	ORG    0x1A00000
	jmp    start
	db     'DEX6'
 ;----------------------------------------------------;
 ; Start of program.                                  ;
 ;----------------------------------------------------;
start:
	mov    ax,18h
	mov    ds,ax
	mov    es,ax
 ;----------------------------------------------------;
 ; Fill the address of calls.                         ;
 ;----------------------------------------------------;
	mov	edi,Functions
	mov	al,0
	mov	ah,0x0a
	int	50h
 ;----------------------------------------------------;
 ; Start message.                                     ;
 ;----------------------------------------------------;
	mov	esi,StartIt
	call	[PrintString_0]
	call	[WaitForKeyPress]
	mov	eax,FileSize-Buffer
	mov	[Length2],eax
	call	ResetDSP
	or	ax,ax
	jz	RstOK
	mov	esi,ResetErrS
	jmp	RstOK1
RstOK:
	mov	esi,ResetOKS
	call	[PrintString_0]
	mov	al,0DDh
	call	MstrVol
	mov	al,0D1h
	call	WriteDSP
	mov	[MemLoc],0x9000
	shl	[MemLoc],4
	mov	eax,65h
	call	[GetIntVector]
	mov	[OldAddress],edx
	mov	eax,65h
	mov	edx,PlayerIRQ
	call	[SetIntVector]
	mov	[BufferAddOn],0
	int	65h
 ;----------------------------------------------------;
 ; Main loop                                          ;
 ;----------------------------------------------------;
WavLoop:
	HLT
	cmp	[Length2],0
	jne	WavLoop
 ;----------------------------------------------------;
 ; Exit loop                                          ;
 ;----------------------------------------------------;
	mov	esi,AnyKey
	call	[PrintString_0]
Done:	call	[WaitForKeyPress]
	mov	eax,65h
	mov	edx,[OldAddress]
	call	[SetIntVector]
	ret

RstOK1:
	call	[PrintString_0]
	jmp	Done

 ;----------------------------------------------------;
 ; DMAPlay                                            ;
 ;----------------------------------------------------;
DMAPlay:    ;uses eax ebx edx
	dec	word [Length1]
	mov	byte [Page1],00h
	mov	al,(Channel+4)
	mov	dx,0Ah
	out	dx,al
	xor	al,al
	mov	dx,0Ch
	out	dx,al
	mov	al,ModeReg
	mov	dx,0Bh
	out	dx,al
	mov	eax,[MemLoc]
	mov	dx,AddPort
	out	dx,al
	xchg	al,ah
	out	dx,al
	mov	eax,[MemLoc]
	mov	edx,eax
	and	eax,65536
	jz	MemLocN1
	inc	byte [Page1]
MemLocN1:
	mov	eax,edx
	and	eax,131072
	jz	MemLocN2
	add	byte [Page1],02
MemLocN2:
	mov	eax,edx
	and	eax,262144
	jz	MemLocN3
	add	byte [Page1],04
MemLocN3:
	mov	eax,edx
	and	eax,524288
	jz	MemLocN4
	add	byte [Page1],08
MemLocN4:
	mov	dx,PgPort
	mov	al,[Page1]
	out	dx,al
	mov	dx,LenPort
	mov	ax,[Length1]
	out	dx,al
	xchg	al,ah
	out	dx,al
	mov	dx,0Ah
	mov	al,Channel
	out	dx,al
	mov	al,40h
	call	WriteDSP
	xor	edx,edx
	mov	eax,1000000
	mov	ebx,Freq
	div	ebx
	mov	ebx,eax
	mov	eax,256
	sub	eax,ebx
	call	WriteDSP
	mov	al,14h
	call	WriteDSP
	mov	ax,[Length1]
	call	WriteDSP
	xchg	al,ah
	call	WriteDSP
	ret

 ;----------------------------------------------------;
 ; MstrVol                                            ;
 ;----------------------------------------------------;
MstrVol:    ;uses ax dx
	push	ax
	mov	dx,(BasePort+4)
	mov	al,22h
	out	dx,al
	pop	ax
	inc	dx
	out	dx,al
	ret

 ;----------------------------------------------------;
 ; ResetDSP                                           ;
 ;----------------------------------------------------;
ResetDSP:   ; uses cx dx
	mov	dx,(BasePort+6)
	mov	al,01
	out	dx,al
	mov	ecx,50
WaitIt1:
	in	al,dx
	loop	WaitIt1
	xor	al,al
	out	dx,al
	mov	ecx,50
WaitIt2:
	in	al,dx
	loop	WaitIt2
	mov	ah,0FFh
	mov	dx,(BasePort+14)
	in	al,dx
	and	al,80h
	cmp	al,80h
	jne	ResetErr
	mov	dx,(BasePort+10)
	in	al,dx
	cmp	al,0AAh
	jne	ResetErr
ResetOK:
	xor	ax,ax
ResetErr:
	ret

 ;----------------------------------------------------;
 ; WriteDSP                                           ;
 ;----------------------------------------------------;
WriteDSP:   ;uses ax dx
	push	eax
	mov	dx,(BasePort+12)
WaitIt:
	in	al,dx
	and	al,80h
	jnz	WaitIt
	pop	eax
	out	dx,al
	ret

 ;----------------------------------------------------;
 ; WriteDSP                                           ;
 ;----------------------------------------------------;
PlayerIRQ:
	pushad
	cmp	[Length2],0
	jbe	ExitNoDMAPlay
	call	UpDateBuffer
	mov	[Length1],0xffff-1
	cmp	[Length2],0xffff-1
	ja	PlayFullBlock
	mov	eax,[Length2]
	mov	[Length1],ax
	mov	[Length2],0
	jmp	PlayWav
PlayFullBlock:
	sub	[Length2],0xffff-1
PlayWav:
	xor	eax,eax
	xor	edx,edx
	call	DMAPlay
	mov	al,[counter]
	call	[WriteHex8]
	inc	[counter]
ExitNoDMAPlay:
	mov	dx,0x22e
	in	al,dx
	mov	al,0x20
	out	0xa0,al
	out	0x20,al
	popad
	iret

 ;----------------------------------------------------;
 ; UpDateBuffer                                       ;
 ;----------------------------------------------------;
UpDateBuffer:
	pushad
	push	es
	mov	ax,8h
	mov	es,ax
	mov	esi,Buffer
	add	esi,[BufferAddOn]
	mov	edi,0x90000
	mov	ecx,0xffff-1 ;FileSize-Buffer
	rep	movsb
	add	[BufferAddOn],0xffff-1
	pop	es
	popad
	ret

 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;

StartIt     db	10,13,'NBASM PlayWav 2012,  A utility for Playing WAV files with SB'
	    db	10,13,'Converted to DexOS, by Craig Bamford (a.k.a Dex)'
	    db	10,13,'Press a key to Test for SB & play wav.',13,0

File1	    db	'test.wav',0
FileErr     db	10,13,'Error opening file...',10,13,0
ResetOKS    db	10,13,'Reset DSP OK',0
ResetErrS   db	10,13,'Error Reseting DSP...',10,13,0
AnyKey	    db	10,13,'Press a key to exit.',10,13,0

Length1     dw	00h
MemLoc	    dd	0000h
Page1	    db	00h
BufferAddOn dd 0
Length2     dd	0
OldAddress  dd 0
counter     db 0
Buffer:
file	    "test.wav"
FileSize:
 ;----------------------------------------------------;
 ; calltable include goes here.                       ;
 ;----------------------------------------------------;
include    'Dex.inc'


