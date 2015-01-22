;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.01                                                       ;;
;; Author           :   Dex                                                         ;;
;; Website          :   www.dex-os.com                                              ;;
;; Forum            :                                                               ;;
;; wiki             :                                                               ;;
;; Date             :   October 10, 2006                                            ;;
;; Filename         :   Load.asm                                                    ;;
;; Assembler Command:   FASM kernel32.asm kernel32.exe                              ;;
;; Copy Right Owners:   Craig Bamford (A.K.A Dex)                                   ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Disclaimer       :                                                               ;;
;; This software is provided "AS IS" without warranty of any kind, either           ;;
;; expressed or implied, including, but not limited to, the implied                 ;;
;; warranties of merchantability and fitness for a particular purpose. The          ;;
;; entire risk as to the quality and performance of this software is with           ;;
;; you.                                                                             ;;
;; In no event will the author's, distributor or any other party be liable to       ;;
;; you for damages, including any general, special, incidental or                   ;;
;; consequential damages arising out of the use, misuse or inability to use         ;;
;; this software (including but not limited to loss of data or losses               ;;
;; sustained by you or third parties or a failure of this software to operate       ;;
;; with any other software), even if such party has been advised of the             ;;
;; possibility of such damages.                                                     ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
format binary as 'dex'
use32
	ORG   0x1A00000 	; where our program is loaded to
	jmp   start		; jump to the start of program.
	db    'DEX6'		; We check for this, to make shore it a valid Dex4u file.

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Start of program.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
start:
	mov   ax,18h		; set ax to nonlinear base
	mov   ds,ax		; add them to ds
	mov   es,ax		; and es.
	push  edi
	mov   edi,Functions				  
	mov   al,0					  
	mov   ah,0x0a				  
	int   50h
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Check for fat module                                                             ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov	esi,ID					  
	call	[ModuleFunction]			  
	jc	NoFatModLoaded				
	mov	[Fatmod],eax 
NoFatModLoaded:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Get filename from command line input.                                            ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop   esi
	cld
	mov   ecx,1
.l1:	lodsb
	test  al,al
	jz    nofile
	cmp   al,' '
	jnz   .l1
	loop  .l1

.l3:	lodsb	    
	test  al,al
	jz    nofile
	cmp   al,' '
	jz    .l3
	dec   esi
	mov   [ModuleNamePointer],esi
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Convert the module name to relocation file name.                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ecx,16
	mov   eax,0
	mov   edi,RelocateBuffer
	cld
	rep   stosb
	mov   edi,RelocateBuffer
	mov   esi,[ModuleNamePointer]
	mov   ecx,9
	mov   [FileNameLen],0
GetRelocteName:
	inc   [FileNameLen]
	lodsb
	cmp   al,"."
	je    FoundChar
	stosb
	loop  GetRelocteName
	jmp   RelocateErrMes
FoundChar:
	add   [FileNameLen],3
	pushad
	mov   ecx,[FileNameLen]
	mov   edi,Mod_name_save
	mov   esi,[ModuleNamePointer]
	rep   movsb
	mov   al,0
	stosb
	popad
	stosb
	mov   eax,'REL '
	stosd
	dec   edi
	mov   eax,0
	stosb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Set where to load the module and relocate file too.                              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   dword[TempModLoadAddrVar],0x9ff9e0
	mov   dword[TempRelocLoadAddrVar],0x900000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load mod  file drive.                                                            ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	call  LoadMod
	jc    nofile
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load Relocate  file drive.                                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	call  LoadRel
	jc    nofile
LoadingFinished:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Check its a module file.                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   esi,dword[TempModLoadAddrVar]
	add   esi,2
	cmp   dword[ds:esi],'MOD1'
	je    ItsaDexMod   
	add   esi,3
	cmp   dword[ds:esi],'MOD1'
	jne   NotaDexMod
ItsaDexMod:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Its a module so clear the 32 byte header buffer.                                 ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   [ModInfoStart],esi
	mov   edi,ModLable
	mov   al,0
	mov   ecx,32
	cld
	rep   stosb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load the module headerinfo to the buffer.                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   esi,[ModInfoStart]
	add   esi,4
	mov   edi,ModLable
	mov   ecx,24
	cld
	rep   movsb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Get the base add on and save it                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	call  [GetBaseAddOn]
	mov   [BaseAddOn],eax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Get the address of the mod list + number of and save to vars                     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	call  [GetModListAddress]
	mov   dword[ModListAddress],edi
	mov   dword[NumberOfModsLoaded],ecx
	cmp   ecx,1
	jbe    NoModsLoaded
	mov   ebx,ecx
	mov   edi,dword[ModListAddress]
	mov   esi,[ModInfoStart]
	add   esi,4
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; See if this module is aready loaded, if so exit with error.                      ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MachModIDLoop:
	mov   ecx,8
	call  [CompareString]
	jnc   ModAlreadyLoaded
	add   edi,32
	dec   ebx
	cmp   ebx,0
	jne   MachModIDLoop
NoModsLoaded:
	call  [TopOfMemory]
	cmp   eax,0x800000
	jbe   NotEnoughMemory
	mov   [TopOfMemoryVar],eax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Get the top of memory, sub size of module and save as load address of module.    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	sub   eax,[SizeOfModule]
	and   eax,0xfffffff0
	mov   [ModuleLoadAddress],eax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Check we have not gone below 8MB.                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	cmp   eax,0x800000
	jbe   NotEnoughMemory
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Up date number of modules loaded.                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ecx,dword[NumberOfModsLoaded]
	inc   ecx
	call  [SetModListNumber]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; We get a error if more than 64 are loaded.                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	jc    MaxMODS64
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Up Date top of Ram.                                                              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   eax,[ModuleLoadAddress]
	sub   eax,16
	call  [UpDateTopOfMemory]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Sub the base add on and up date the module load address.                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ecx,[BaseAddOn]
	sub   [ModuleLoadAddress],ecx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Move the module to its permenant address.                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   edi,[ModuleLoadAddress]
	mov   esi,dword[TempModLoadAddrVar]
	mov   ecx,[FddfileSizeInBytes]
	cld
	rep   movsb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Up date the new header info.                                                     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ecx,[ModuleLoadAddress]
	add   [ModLoadPtr],ecx
	add   [ModUnLoad],ecx
	mov   ecx,dword[ModuleLoadAddress]
	mov   dword[AbsoluteAddress],ecx
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Up date the Module list with the New info.                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ecx,dword[NumberOfModsLoaded]
	cmp   ecx,0
	je    @f
	shl   ecx,5
@@:
	mov   edi,dword[ModListAddress]
	add   edi,ecx
	mov   esi,ModLable
	mov   ecx,32
	cld
	rep   movsb
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; parse header.                                                                    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   esi,dword[TempRelocLoadAddrVar]
	xor   ecx,ecx
	mov   edx,[ModuleLoadAddress]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Loop and add the new load address offset.                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
.relloop:	
	lodsd			 ; load 32 bit base offset
	cmp   eax,0xFFFFFFFF	 ; end?
	jz   .donereloc
	mov   ebx,eax		 ; ebx = base offset
	mov   eax,[ds:edx+ebx]	 ; get dword to patch in binary image
	add   eax,edx		 ; add program address to it
	mov   [ds:edx+ebx],eax	 ; store it back
	jmp   .relloop		 ; repeat until end of relocations
.donereloc:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Set regs to a none state.                                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	xor   eax,eax
	mov   ebx,eax
	mov   ecx,eax
	mov   edx,eax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Jump to the image or in this case call.                                          ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	call  dword[ModuleLoadAddress]
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 1.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ModAlreadyLoaded:
	mov   esi,ModAlreadyLoadedErr
	call  [PrintString_0]
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 2.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NotaDexMod:
	mov   esi,ErrorNotAmodFile
	call  [PrintString_0]
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 3.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
nofile: mov   esi,ErrorNoFile
	call  [PrintString_0]
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 4.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NotEnoughMemory:
	mov   esi,NotEnoughMemoryErr
	call  [PrintString_0]
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 5.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MaxMODS64:
	mov   esi,MaxMODS64Err
	call  [PrintString_0]
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 5.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RelocateErrMes:
	mov   esi,RelocateErr
	call  [PrintString_0]
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; LoadMod.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LoadMod:
;==================================================================================  ;
;  Open file.                                                                        ;
;==================================================================================  ;
	pushad
	mov	edx,Mod_name_save
	mov	eax,9
	call	[Fatmod]				  
	jc	FddErr	
	xor	ebx,ebx 				  
	mov	bx,ax					  
	mov	dword[LoadMod_handle],ebx		  
;===================================================================================  ;
;  Read file.                                                                         ;
;===================================================================================  ;
	mov	edx,dword[TempModLoadAddrVar]
	mov	eax,11
	call	[Fatmod]							       
	jc	FddErr22
	mov	[FddfileSizeInBytes],eax
;===================================================================================  ;
;  Close file.                                                                        ;
;===================================================================================  ;
	mov	ebx,dword[LoadMod_handle]		  
	mov	eax,10
	call	[Fatmod]				     
	popad
	clc
	ret

;===================================================================================  ;
;  Error and Close file.                                                              ;
;===================================================================================  ;
FddErr22:
	mov	ebx,dword[LoadMod_handle]		  
	mov	eax,10
	call	[Fatmod]
;===================================================================================  ;
;  Error.                                                                             ;
;===================================================================================  ;
FddErr:
	popad
	stc
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Load relocate info  file.                                                        ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
LoadRel:
;==================================================================================  ;
; Open file.                                                                         ;
;==================================================================================  ;
	pushad
	mov	edx,RelocateBuffer
	mov	eax,9
	call	[Fatmod]				  
	jc	FddErr	
	xor	ebx,ebx 				  
	mov	bx,ax					  
	mov	dword[LoadMod_handle],ebx 
;==================================================================================  ;
;  open file.                                                                        ;
;==================================================================================  ;
	mov	edx,dword[TempRelocLoadAddrVar] 		     
	mov	eax,11
	call	[Fatmod]							       
	jc	FddErr22
;==================================================================================  ;
;  Close file.                                                                       ;
;==================================================================================  ;
	mov	ebx,dword[LoadMod_handle]		  
	mov	eax,10
	call	[Fatmod]				    
	popad
	clc
	ret

Mod_load:
	stc
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Data.                                                                            ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
Mod_name_save:	     db '                      ',0
ID		     db 'DEXOSFAT',0
LoadMod_handle	     dd  0
Fatmod		     dd  Mod_load
ErrorNoFile:	     db  'Cannot open file (or no file specified)',13,0
ErrorNotAmodFile:    db  'Not a MOD1 file ',13,0
ModAlreadyLoadedErr  db  'Driver already loaded',13,0
NotEnoughMemoryErr   db  'Not enough memory',13,0
MaxMODS64Err	     db  'Error, Over 64 modules already loaded (Max 64)',13,0
RelocateErr	     db  'Error, Only 8 letters + ext allowed, eg: MyModule.dex',13,0

ModListAddress	     dd 0
NumberOfModsLoaded   dd 0
ModuleLoadAddress    dd 0

LoadDriveLetter      db 0
FileNameLen	     dd 0
ModuleNamePointer    dd 0
FddfileSizeInBytes   dd 0

TopOfMemoryVar	     dd 0
TopOfMemoryVar1      dd 0
TempRelocLoadAddrVar dd 0
ModLoadAddrVar	     dd 0
TempModLoadAddrVar   dd 0
ModInfoStart	     dd 0
BaseAddOn	     dd 0

ModLable:
ModID			db  '        '		    ; This the ID of the module.
SizeOfModule		dd 0			    ; This put at end of module.
ModLoadPtr		dd 0			    ; This is to run code on first load of module.
ModUnLoad		dd 0			    ; This is to run code on unload of module.
NumberOfModFunctionsPtr dd 0			    ; This points to the function list.
AbsoluteAddress 	dd 0			    ; This points to the module load address.
Packer			dd 0			    ; Just to pack it out

RelocateBuffer: times  128  db 0

include 'DeXX.inc'				    ; Here is where we includ our "DeXX.inc" file
