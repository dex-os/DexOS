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

	mov  dword[TempModLoadAddrVar],0x1900000 ;0x0dfff360  ;0x5ff9e0      ; temp load  address of mod

	; load main file
	mov edi,dword[TempModLoadAddrVar]	      ; 0x5ff9e0
	call [FloppyfileLoad]
	call dword[TempModLoadAddrVar]

nofile:
	ret

TempModLoadAddrVar dd 0


include 'DeXX.inc'	; Here is where we includ our "DeXX.inc" file