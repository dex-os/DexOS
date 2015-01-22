;=========================================================;
; Fat module loader                            01/04/2012 ;
;---------------------------------------------------------;
;                                                         ;
; DexOS V0.05                                             ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
FatModLoad:
	pushad
	push	es
	push	ds
	mov	ax,18h
	mov	ds,ax
	mov	es,ax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Set where to load the module and relocate file too.                              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   dword[TempModLoadAddrVar],0x9ff9e0
	mov   dword[TempRelocLoadAddrVar],0x900000
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; load the module and relocate file.                                               ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   esi,Fat_module_dri
	mov   ecx,Fat_module_driEnd-Fat_module_dri
	mov   [FddfileSizeInBytes],ecx
	mov   edi,dword[TempModLoadAddrVar]
	cld
	rep   movsb

	mov   esi,Fat_module_rel
	mov   ecx,Fat_module_relEnd-Fat_module_rel
	mov   edi,dword[TempRelocLoadAddrVar]
	rep   movsb
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
	call  GetBaseAddOn  ;;
	mov   [BaseAddOnML],eax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Get the address of the mod list + number of and save to vars                     ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	call  GetModListAddress     ;;
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
	call  CompearString	   ;;
	jnc   ModAlreadyLoaded
	add   edi,32
	dec   ebx
	cmp   ebx,0
	jne   MachModIDLoop
NoModsLoaded:
	call  TopOfMemory	   ;;
	cmp   eax,0x800000
	jbe   NotEnoughMemory
	mov   [TopOfMemoryVar],eax
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Get the top of memory, sub size of module and save as load address of module.    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	sub   eax,[SizeOfModuleML]
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
	call  SetModListNumber	  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; We get a error if more than 64 are loaded.                                       ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	jc    MaxMODS64
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Up Date top of Ram.                                                              ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   eax,[ModuleLoadAddress]
	sub   eax,16
	call  UpDateTopOfMemory    ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Sub the base add on and up date the module load address.                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ecx,[BaseAddOnML]
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
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Return.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop	ds
	pop	es
	popad
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 1.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
ModAlreadyLoaded:
	mov   esi,ModAlreadyLoadedErr
	call  print_string_zero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Return.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop	ds
	pop	es
	popad
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 2.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NotaDexMod:
	mov   esi,ErrorNotAmodFile
	call  print_string_zero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Return.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop	ds
	pop	es
	popad
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 3.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
nofile: mov   esi,ErrorNoFile
	call  print_string_zero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Return.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop	ds
	pop	es
	popad
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 4.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
NotEnoughMemory:
	mov   esi,NotEnoughMemoryErr
	call  print_string_zero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Return.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop	ds
	pop	es
	popad
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 5.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MaxMODS64:
	mov   esi,MaxMODS64Err
	call  print_string_zero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Return.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop	ds
	pop	es
	popad
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Error messages 5.                                                                ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RelocateErrMes:
	mov   esi,RelocateErr
	call  print_string_zero
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;  Return.                                                                         ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	pop	ds
	pop	es
	popad
	ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Data.                                                                            ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
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
BaseAddOnML	     dd 0

ModLable:
ModIDa			db  '        '		    ; This the ID of the module.
SizeOfModuleML		dd 0			    ; This put at end of module.
ModLoadPtr		dd 0			    ; This is to run code on first load of module.
ModUnLoad		dd 0			    ; This is to run code on unload of module.
NumberOfModFunctionsPtr dd 0			    ; This points to the function list.
AbsoluteAddress 	dd 0			    ; This points to the module load address.
Packer			dd 0			    ; Just to pack it out

RelocateBuffer: times  128  db 0


