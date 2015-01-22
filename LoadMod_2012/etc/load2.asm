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

	mov  dword[TempModLoadAddrVar],0xdcff9e0 ;0x0dfff360  ;0x5ff9e0      ; temp load  address of mod

	; load main file
	mov edi,dword[TempModLoadAddrVar]	      ; 0x5ff9e0
	call [FloppyfileLoad]

	mov   esi,dword[TempModLoadAddrVar]	      ;0x5ff9e0
	add   esi,2
	cmp   dword[ds:esi],'DEX1'
	je    ItsaDexMod   
	add   esi,3
	cmp   dword[ds:esi],'DEX1'
	jne   NotaDexMod



ItsaDexMod:
	mov   byte [fs:0xB809E], "Y"
	CALL  [WaitForKeyPress]

	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	xor   eax,eax
	mov   ebx,eax
	mov   ecx,eax
	mov   edx,eax
	call dword[TempModLoadAddrVar]
	ret
      ;  jmp 10h:0xf00000                    ; [TempModLoadAddrVar]
NotaDexMod:
	mov   byte [fs:0xB809E], "N"

	CALL  [WaitForKeyPress]
	ret
nofile:
	mov   byte [fs:0xB809E], "E"

	CALL  [WaitForKeyPress]
	ret

TempModLoadAddrVar dd 0


include 'DeXX.inc'	; Here is where we includ our "DeXX.inc" file