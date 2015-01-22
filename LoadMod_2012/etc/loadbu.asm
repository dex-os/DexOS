use32
	ORG   0x400000		; where our program is loaded to
	jmp   start		; jump to the start of program.
	db    'DEX1'		; We check for this, to make shore it a valid Dex4u file.

start:
	mov   ax,18h		; set ax to nonlinear base
	mov   ds,ax		; add them to ds
	mov   es,ax		; and es.

	mov   edi,Functions	; this is the interrupt
	mov   al,0		; we use to load the DexFunction.inc
	mov   ah,0x0a		; with the address to dex4u functions.
	int   40h 
      
	; get filename
	call  [GetParams]
	cld
	mov ecx,1
.l1:	lodsb
	test al,al
	jz nofile
	cmp al,' '
	jnz .l1
	loop .l1

.l3:	lodsb	    
	test al,al
	jz nofile
	cmp al,' '
	jz .l3	      
	dec esi

	mov  dword[TempModLoadAddrVar],0xdfff360-0x100000 ;0x5ff9e0      ; temp load  address of mod
	mov   dword[TempRelocLoadAddrVar],0x500000    ; temp load address of relocate file
	; load main file
	mov edi,dword[TempModLoadAddrVar]	      ; 0x5ff9e0
	call [FloppyfileLoad]
	jc nofile

	; load relocate info  file
	mov esi,GetFile
	mov edi,dword[TempRelocLoadAddrVar]	       ; 0x500000
	call [FloppyfileLoad]
	jc nofile
;-----------------------------------------------
	mov   esi,dword[TempModLoadAddrVar]	      ;0x5ff9e0
	add   esi,2
	cmp   dword[ds:esi],'MOD1'
	je    ItsaDexMod   
	add   esi,3
	cmp   dword[ds:esi],'MOD1'
	jne   NotaDexMod
ItsaDexMod:
	mov   [ModInfoStart],esi
	mov   edi,ModLable
	mov   al,0
	mov   ecx,32
	cld
	rep   stosb
	mov   esi,[ModInfoStart]
	add   esi,4
	mov   edi,ModLable
	mov   ecx,32
	cld
	rep   movsb

	call  [GetBaseAddOn]
	mov   [BaseAddOn],eax
	call  [GetModListAddress]
	mov   dword[ModListAddress],edi
	mov   dword[NumberOfModsLoaded],ecx
	cmp   ecx,1
	jbe    NoModsLoaded
	mov   ebx,ecx
	mov   edi,dword[ModListAddress]
       ; add   edi,32
	mov   esi,[ModInfoStart]
	add   esi,4
      ;  dec   ebx
MachModIDLoop:
	mov   ecx,8 
	call  [CompearString]
	jnc   ModAlreadyLoaded
	add   edi,32
	dec   ebx
	cmp   ebx,0
	jne   MachModIDLoop
NoModsLoaded:
	call  [TopOfMemory]
	mov   [TopOfMemoryVar],eax
;        mov   [TopOfMemoryVar1],eax
;        mov   ebx,16
;        div   ebx
;        SHL   EAX,4
	sub   eax,[SizeModuleInBytes_LM]
	mov   [ModuleLoadAddress],eax
	cmp   eax,0x800000
	jbe   NotEnoughMemory
				      ; call  [WaitForKeyPress]
;---------------------------------
	mov   ecx,dword[NumberOfModsLoaded]
	inc   ecx
	call  [SetModListNumber]
	jc    MaxMODS64
	mov   ecx,[BaseAddOn]
	sub   [ModuleLoadAddress],ecx

sub  [ModuleLoadAddress],-0x100002
mov eax,[ModuleLoadAddress]
call [WriteHex32]
      ;  mov   [ModuleLoadAddress],0xc00000 ;0x6400000 ;0x5ff9e0
       ; add   [ModuleLoadAddress],1

      ;  mov   edi,[ModuleLoadAddress]
      ;  mov   esi,dword[TempModLoadAddrVar]
     ;   mov   ecx,[SizeModuleInBytes_LM]
     ;   cld
     ;   rep   movsb

      ;  mov   edi,[ModuleLoadAddress]
      ;  mov   esi,dword[TempModLoadAddrVar] ;0x5ff9e0
      ;  mov   ecx,[SizeModuleInBytes_LM]
      ;  shr   ecx,2
      ;  cld
      ;  rep   movsd
	mov   ecx,[ModuleLoadAddress]
	add   [AbsoluteAddressInMemory_LM],ecx
	add   [OffsetModLoadCode_LM],ecx
	add   [OffsetModUnLoadCode_LM],ecx
	add   [OffsetServiceTable_LM],ecx
;---------------------------------
				     ;  call  [WaitForKeyPress]
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

;-----

;-----------------------------------------------
	; parse header

	mov esi,dword[TempRelocLoadAddrVar]		;0x500000     ; start of relocs
	xor ecx,ecx
	mov edx,[ModuleLoadAddress]	;0x600000 ; start of image

.relloop:	
	lodsd		     ; load 32 bit base offset
	cmp eax,0xFFFFFFFF   ; end?
	jz .donereloc	   
	mov ebx,eax	     ; ebx = base offset
	mov eax,[edx+ebx]    ; get dword to patch in binary image
	add eax,edx	     ; add program address to it
	mov [edx+ebx],eax    ; store it back
	jmp .relloop	     ; repeat until end of relocations
.donereloc:

	call [WaitForKeyPress]
	mov edx,[ModuleLoadAddress] ;[OffsetModLoadCode_LM]  ;0x5ff9e0     ; 0x600000
	jmp edx
       ; call  [ModuleLoadAddress]
       ; ret
ModAlreadyLoaded:
	mov esi,ModAlreadyLoadedErr
	call [PrintString]
	ret

NotaDexMod:
	mov esi,ErrorNotAmodFile
	call [PrintString]
	ret

nofile: mov esi,ErrorNoFile
	call [PrintString]
	ret

NotEnoughMemory:
	mov esi,NotEnoughMemoryErr
	call [PrintString]
	ret
MaxMODS64:
	mov esi,MaxMODS64Err
	call [PrintString]
	ret
GetFile:	     db  'reloc.bin',0
ErrorNoFile:	     db  'Cannot open file (or no file specified)',13,0
ErrorNotAmodFile:    db  'Not a MOD1 file ',13,0
ModAlreadyLoadedErr  db  'Driver already loaded',13,0
NotEnoughMemoryErr   db  'Not enough memory',13,0
MaxMODS64Err	     db  'Error, Max of 64 drivers, can be loaded at a time',13,0

ModListAddress	     dd 0
NumberOfModsLoaded   dd 0
ModuleLoadAddress    dd 0

TopOfMemoryVar	     dd 0
TopOfMemoryVar1      dd 0
TempRelocLoadAddrVar dd 0
ModLoadAddrVar	     dd 0
TempModLoadAddrVar   dd 0
ModInfoStart	     dd 0
BaseAddOn	     dd 0

ModLable:
ModID_LM				  db '        '
NumberOfFunctions_LM			  dd 0
AbsoluteAddressInMemory_LM		  dd 0
SizeModuleInBytes_LM			  dd 0
OffsetModLoadCode_LM			  dd 0
OffsetModUnLoadCode_LM			  dd 0
OffsetServiceTable_LM			  dd 0

include 'DeXX.inc'	; Here is where we includ our "DeXX.inc" file