;=========================================================;
; Project          :   DexOS  (32bit x86 learning OS)     ;
;=========================================================;
;                  ;                                      ;
; Ver              :   00.06                              ;
; Author           :   Craig Bamford                      ;
; Website          :   www.dex-os.com                     ;
; Forum            :   http://dex.7.forumer.com/          ;
; Date             :   04/04/2012                         ;
; Filename         :   Kernel32.asm                       ;
; Assembler Command:   Fasm kernel32.asm kernel32.exe     ;
; Copy Right Owner :   Craig Bamford                      ;
; licence see..    ;   DexOSLicence.txt                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;                            
; Disclaimer                                              ;
; This software is provided "AS IS" without warranty      ;
; of any kind, either expressed or implied, including,    ;
; but not limited to, the implied warranties of           ;
; merchantability and fitness for a particular purpose.   ;
; The entire risk as to the quality and performance of    ;
; this software is with you.                              ;                                                 ;
; In no event will the author's, distributor or any       ;
; other party be liable to you for damages,               ;
; including any general, special, incidental or           ;
; consequential damages arising out of the use,           ;
; misuse or inability to use this software                ;
; (including but not limited to loss of data or losses    ;
; sustained by you or third parties or a failure of       ;
; this software to operate with any other software),      ;
; even if such party has been advised of the possibility  ;
; of such damages.                                        ;
;                                                         ;
;=========================================================;

;=======================================================  ;
;  File type.                                             ;
;=======================================================  ; 
	format MZ					  ;
	use16						  ;
;=======================================================  ;
;  16-bit real mode.                                      ;
;=======================================================  ; 
	jmp	start					  ;
	include 'RealMode\RmBuf.inc'			  ;
	include 'RealMode\Vesa2_info.inc'		  ;
	include 'RealMode\386.inc'			  ;
	include 'RealMode\A20.inc'			  ;
	include 'RealMode\vesa2.inc'			  ;
	include 'RealMode\RmodeInfo.inc'		  ;
	include 'RealMode\RmFunctions.inc'		  ;
	include 'RealMode\RmMouse.inc'			  ;
;=======================================================  ;
;  End of realmode Include Files.                         ;
;=======================================================  ; 
start:							  ;
	push	cs					  ;
	pop	ds					  ;
	push	ds					  ;
	pop	es					  ;
	push	es					  ;
	pop	ss					  ;
	mov	sp,stackBuff1				  ; Note: we should test for dos before calling
	mov	[BootNumber],dl 			  ; get boot number
	mov	[BootProTest],dh			  ; see if we booted from bootprog
	call	Enable_A20				  ; do the A20
	call	ReadPalette				  ;
	mov	ecx,0xffff				  ;
  A20_delay:						  ; seem from my
	nop						  ; tests we need
	nop						  ; a delay
	loop	A20_delay				  ;
	call	Test4dDos				  ;
	call	ConvMemory				  ;
	call	GetRamSize				  ;
	call	DetectPcIBus				  ;
	call	Vesa					  ;
	call	setmouse				  ;                                                  
	xor	ebx,ebx 				  ;
	mov	bx,ds					  ;         
	shl	ebx,4					  ;         
	mov	[BaseAddOn],ebx 			  ;         
	mov	eax,ebx 				  ;
	mov	[sys_code_1 + 2],ax			  ;          
	mov	[sys_data_1 + 2],ax			  ;
	mov	[Real_code_1 + 2],ax			  ;                  
	mov	[Real_data_1 + 2],ax			  ;
	shr	eax,16					  ;
	mov	[sys_code_1 + 4],al			  ;
	mov	[sys_data_1 + 4],al			  ;
	mov	[Real_code_1 + 4],al			  ;
	mov	[Real_data_1 + 4],al			  ;
	mov	[sys_code_1 + 7],ah			  ;
	mov	[sys_data_1 + 7],ah			  ;
	mov	[Real_code_1 + 7],ah			  ;
	mov	[Real_data_1 + 7],ah			  ;
	add	ebx,gdt 				  ;               
	mov	[gdtr + 2],ebx				  ;
	add	ebx,idt - gdt				  ;               
	mov	[idtr + 2],ebx				  ;
	cli						  ;
	mov	ax,cs					  ;
	mov	[RealModeCS],ax 			  ;
	lgdt	[gdtr]					  ;                
	lidt	[idtr]					  ;
	mov	eax,cr0 				  ;       
	or	al,1					  ;
	mov	cr0,eax 				  ;
	jmp	sys_code:do_pm				  ;
;=======================================================  ;
;  32-bit protected mode.                                 ;
;=======================================================  ;                                 
	use32						  ;
  do_pm:						  ;
	xor	edi,edi 				  ;
	xor	esi,esi 				  ;
	mov	ax,sys_data				  ;
	mov	ds,ax					  ;
	mov	ss,ax					  ;
	nop						  ;
	mov	es,ax					  ;
	mov	gs,ax					  ;
	mov	ax,8h					  ;
	mov	fs,ax					  ;
	mov	esp,stackBuff1				  ;
	jmp	inc_over2				  ;
;=======================================================  ;
; Realmode Var's.                                         ;
;=======================================================  ;    
  RealModeIP:						  ;
		dw 0					  ;
  RealModeCS:						  ;
		dw 0					  ;
  ridtr:	dw 0xFFFF				  ;
		dd 0					  ;
;=======================================================  ;
; global descriptor table (GDT).                          ;
;=======================================================  ;   
  gdtr:        dw gdt_end - gdt - 1			  ;  
	       dd gdt					  ;
	include 'Kernel\Gdt.inc'			  ;
;=======================================================  ;
; interrupt descriptor table (IDT).                       ;
;=======================================================  ;   
  idtr:       dw idt_end - idt - 1			  ; 
	      dd idt					  ;
	include 'Kernel\Idt.inc'			  ;
;=======================================================  ;
; Include Files.                                          ;
;=======================================================  ;
	include 'Kernel\Pic.inc'			  ;
	include 'RmInt\RmInt.inc'			  ;
	include 'Kernel\Isr.inc'			  ;
	include 'Biosint\int10h.inc'			  ;
	include 'KeyBoard\keyboard.inc' 		  ;
	include 'KeyBoard\Gameskey.inc' 		  ;
	include 'FunctionPM\Info.inc'			  ;
	include 'FunctionPM\KernelFunctions.inc'	  ;
	include 'String\String.inc'			  ;
	include 'String\Prompt.inc'			  ;
	include 'FunctionPM\FunctionPM.inc'		  ;
	include 'FunctionPM\TimeDate.inc'		  ;
	include 'FunctionPM\Convert.inc'		  ;
	include 'FunctionPM\Function.inc'		  ;
	include 'FunctionPM\Pci.inc'			  ;
	include 'FunctionPM\PciConstants.inc'		  ;
	include 'Drivers\HddFunctions.inc'		  ;
	include 'Drivers\HddVar.inc'			  ;
	include 'Drivers\ModLoad.inc'			  ;
	include 'FunctionPM\MouseFunction.inc'		  ;
	include 'FunctionPM\PmStartUpSettings.inc'	  ;
	include 'KeyBoard\keymapUK.inc' 		  ;
;=======================================================  ;
; Include File End.                                       ;
;=======================================================  ; 
  inc_over2:						  ;
	call	remap_pic				  ;
	call	unmask_irqs				  ;
	mov	dword[CliLoadAddrVar],0x400000		  ;
	mov	dword[ImageLoadAddrVar],0x800000	  ; Address to load exe to
	mov	byte[CliOK],0				  ;
	mov	[keyBoardStatus],0xb0			  ; this crashs some PC, If so try dehighlighting this.
	call	SetKeyBoardLeds 			  ; and this
	call	SetMemoryVars				  ;
	call	TopOfMemory				  ;
	mov	dword[AmountOfRam],eax			  ;
;=======================================================  ;
; Set drive and load Fat.                                 ;
;=======================================================  ; 
	call	GetBootDrive				  ; Test for A: OR C: drive
	jc	FatError				  ;
;=======================================================  ;
; Print load message.                                     ;
;=======================================================  ; 
	call	cls_text				  ;
	call	ReadyPrompt				  ;
	call	Prompt					  ;
	call	SetPath 				  ;
	call	LoadFatRoot				  ;
	jnc	EnoughMemoryTest			  ;
;=======================================================  ;
; Error Loading GUI.                                      ;
;=======================================================  ; 
FatError:						  ;
	mov	byte [fs:0xB809E],"E"			  ;
	jmp	$					  ;
;=======================================================  ;
; NotEnoughMemory print message.                          ;
;=======================================================  ;
ErrorNEM:						  ;
	mov	esi,NemMessage				  ;
	call	print_string_zero			  ;
LetUsLoopTheLoop:					  ;
	hlt						  ;
	jmp	LetUsLoopTheLoop			  ;
;=======================================================  ;
; Enough Memory test.                                     ;
;=======================================================  ;
EnoughMemoryTest:					  ;
	mov	eax,dword[AmountOfRam]			  ;
	cmp	eax,0x4000000				  ;
	jb	ErrorNEM				  ;
FatOK:							  ;
;=======================================================  ;
; Load FAT module.                                        ;
;=======================================================  ;
	call   FatModLoad				  ;
;=======================================================  ;
; Load CLI.                                               ;
;=======================================================  ;
Load_CLI:						  ;
	mov	edi,CliName				  ;
	mov	edx,dword[CliLoadAddrVar]		  ;
	call	RunCommand				  ;
	jc	FatError				  ;
	mov	byte[CliOK],1				  ; 
;=======================================================  ;
; Run CLI.                                                ;
;=======================================================  ;
CliAlReadyLoaded:					  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	xor	eax,eax 				  ;
	mov	ebx,eax 				  ;
	mov	ecx,eax 				  ;
	mov	edx,eax 				  ;
	mov	esi,eax 				  ;
	mov	edi,eax 				  ;
	call	dword[CliLoadAddrVar]			  ;
	mov	ax,18h					  ;             
	mov	ds,ax					  ;
	mov	es,ax					  ;
	xor	eax,eax 				  ;
	mov	esi,eax 				  ;
	mov	edi,eax 				  ;
	mov	ebx,eax 				  ;
	mov	ecx,eax 				  ;
	mov	edx,eax 				  ;
	mov	[keybuffer],0				  ;
	cmp	byte[CliOK],1				  ; 
	je	CliAlReadyLoaded			  ;        
;=======================================================  ;
; Should not get here                                     ;
;=======================================================  ; 
  ShouldNotGetHere:					  ;
	hlt						  ;
	jmp	ShouldNotGetHere			  ;
;=======================================================  ;
; data.                                                   ;
;=======================================================  ;
AmountOfRam:	   dd 0 				  ;
NemMessage:	   db 10,13				  ;
		   db 'Error: not enough memory, dex-os ' ;
		   db 'needs aleast 64MB of ram',0	  ;
							  ;
CliName:	   db 'cli.bin',0			  ;
;=======================================================  ;
; Fat module driver.                                      ;
;=======================================================  ;
align 4 						  ;
Fat_module_dri: 					  ;
file   'Drivers\fatmod.dri'				  ;
Fat_module_driEnd:					  ;
rd 1							  ;
;=======================================================  ;
; Fat module Rel.                                         ;
;=======================================================  ;
Fat_module_rel: 					  ;
file   'Drivers\fatmod.rel'				  ;
Fat_module_relEnd:					  ;
rd 1							  ;
;=======================================================  ;
; this needs to be the last file.                         ;
;=======================================================  ;
	include 'Drivers\Fat.inc'			  ;
	include 'Drivers\Fat12.inc'			  ;
	include 'Drivers\Fat16.inc'			  ;
	include 'Drivers\FatVars.inc'			  ;
	include 'Module\ModList.inc'			  ;
