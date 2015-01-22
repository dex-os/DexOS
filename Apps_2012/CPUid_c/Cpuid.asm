;=========================================================;
; CPUID                                        20/04/2012 ;
;---------------------------------------------------------;
; CPUID Demo.                                             ;
;                                                         ;
; DexOS V6                                                ;
; (c) Craig Bamford, All rights reserved.                 ;                          
;=========================================================;
format binary as 'dex'
use32
	ORG   0x1A00000 				; where our program is loaded to
	jmp   start					; jump to the start of program.
	db    'DEX6'					; We check for this, to make shore it a valid DexOS file.

msg1:	db " Error!, Needs a Pentium or later.",10,13
	db " Press any key to exit. ",10,13,0
msg2:	db " MTRR Set to write combine sucsessfully.",10,13
	db " Press any key to exit. ",10,13,0
msg3:	db " Do you want to Set MTRR to write combine, Y/N ?.",10,13,0
msg4:	db " Error!, MTRR Not available.",10,13
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
 ;----------------------------------------------------;
 ; Fill vesa info out.                                ;
 ;----------------------------------------------------;
	call [LoadVesaInfo]
	mov   edi,VESA_Info
	mov   ecx,193
	cld
	cli
	rep   movsd
	sti
 ;----------------------------------------------------;
 ; Print start message.                               ;
 ;----------------------------------------------------;
	mov   esi,CPUlogo
	call  [PrintString_0]
 ;----------------------------------------------------;
 ; Test What processor                                ;
 ;----------------------------------------------------;
	call  TestProcessor
	jc    ExitCpuidError
 ;----------------------------------------------------;
 ; Print Vendor                                       ;
 ;----------------------------------------------------;
	mov   esi,VendorMess
	call  [PrintString_0]
	call  PrintVeatures
 ;----------------------------------------------------;
 ; Exit Cpuid test.                                   ;
 ;----------------------------------------------------;
	mov   esi,NextLine
	call  [PrintString_0]
	mov   esi,msg3
	call  [PrintString_0]
YNMessageLoop:	
	call  [WaitForKeyPress]      
	cmp   al,'n'
	je    ExitCpuidNoMsg
	cmp   al,'N'
	je    ExitCpuidNoMsg
	call  MtrrSetUp
	jc    NoMtrrMsgError
	jmp   NoMtrrMsgSucsess	  
ExitCpuid:
	mov   eax,2
	call  [SetDelay]
	;call  [WaitForKeyPress]
ExitCpuidNoMsg:
	ret

 ;----------------------------------------------------;
 ; Exit Cpuid test error.                             ;
 ;----------------------------------------------------;
ExitCpuidError:
	mov   esi,msg1
	call  [PrintString_0]
	jmp   ExitCpuid 
 ;----------------------------------------------------;
 ; Exit Cpuid test error.                             ;
 ;----------------------------------------------------;
NoMtrrMsgError:
	mov   esi,msg4
	call  [PrintString_0]
	jmp   ExitCpuid 
 ;----------------------------------------------------;
 ; Exit Cpuid test error.                             ;
 ;----------------------------------------------------;
NoMtrrMsgSucsess:
	mov   esi,msg2
	call  [PrintString_0]
	jmp   ExitCpuid 


TestProcessor:
 ;====================================================;
 ; Test What processor                                ;
 ;====================================================;
	pushfd					      
	pop	eax
	mov	ecx,eax
	xor	eax,0x00200000			      
	push	eax
	popfd
	pushfd					      
	pop	eax
	push	ecx				     
	popfd
	and	eax,0x00200000			      
	and	ecx,0x00200000			      
	cmp	eax,ecx
	jz	NotaPentium
 ;----------------------------------------------------;
 ;  Pentium or later                                  ;
 ;----------------------------------------------------;
	mov eax,0
	cpuid
	mov [VendorId],ebx
	mov [VendorId+4],edx
	mov [VendorId+8],ecx
 ;----------------------------------------------------;
 ;  Get Version & Features info                       ;
 ;----------------------------------------------------;
	mov eax,1
	cpuid
	mov [Version],eax
	mov [Veatures],edx
 ;----------------------------------------------------;
 ; Cpuid done                                         ;
 ;----------------------------------------------------;
Cpuiddone:
	ret

NotaPentium:  
	stc	 
	ret

 ;====================================================;
 ; MtrrSetUp                     (for  write combine) ;
 ;====================================================;
MtrrSetUp:
	pushad
	mov   edx,[Veatures]
	test  edx,1000000000000b
	jz    NoMtrr
	call  FindEmptyMtrr
	jc    NoMtrr
	mov   edx,0x0			  
	mov   eax,[ModeInfo_PhysBasePtr]	       ; NOTE: This is vesa2 LFB address
	or    eax,1
	wrmsr
	inc   ecx
	mov   edx,0xf
	mov   eax,0xff800800
	wrmsr
	mov   ecx,0x2ff 		  
	rdmsr
	or    eax,100000000000b 	  
	wrmsr
	popad
	ret

NoMtrr:
	popad
	stc	 
	ret

 ;====================================================;
 ; FindEmptyMtrr.                                     ;
 ;====================================================;
FindEmptyMtrr:
       mov    ecx,0x201-2
@@:
	add    ecx,2
	cmp    ecx,0x200+8*2
	jge    ErrorNoFreeMtrr
	rdmsr
	test   eax,0x0800
	jnz    @b
	dec    ecx
	ret
ErrorNoFreeMtrr:      
	stc  
	ret

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;
VERSION_STRING equ "0.01"
ProgLogo db ' DexOS MTRR setting program,, version ',VERSION_STRING,10,13,13
      db " Press anykey to Test for Pentium or later.",10,13,0

VendorMess     db  0xd2,0x02,'  CPU Vendor ',0xd2,0x09
VendorId       rd	     12
StringEnd      db  0	
NextLine       db  '',10,13,13,0
Version        dd	     0
Veatures       dd	     0
include 'Text.inc'
include 'CPUinfo.inc'
include 'Dex.inc'
