
; we have the COFF file loaded in buffer, now we should write the program
; code to [output1] handle, and relocations to [output2] handle

	mov	esi,buffer
	cmp	word [esi],0x14C
	jne	invalid_format
	movzx	ecx,word [esi+2]	; number of sections
	movzx	eax,word [esi+10h]
	lea	esi,[esi+14h+eax]	; skip headers
	xor	eax,eax
    find_flat_section:
	inc	eax
	cmp	dword [esi],'.fla'
	jne	next_section
	cmp	dword [esi+4],'t'
	jne	next_section
	mov	ebp,eax

	mov	eax,[esi+14h]
	lea	edx,[buffer+eax]	; pointer to section data

	pushad				; Added by Dex
	mov	[No2CUT_Found],0	;
	mov	ebx,[esi+10h]		;
	shr	ebx,2			;
	mov	ecx,ebx 		;
	xor	eax,eax 		;
TryAgain:				;
	cmp	dword [edx],'2CUT'	;
	je	Found			;
	add	edx,4			;
	add	eax,4			;
	loop	TryAgain		;
	mov	[No2CUT_Found],1	;
	jmp	No2CutFound		;
Found:					;
	mov	dword [esi+10h],eax	;
No2CutFound:				;
	popad				;

	invoke	WriteFile,[output1],edx,dword [esi+10h],bytes_count,0

	CMP WORD [ESI+$20],0		; Added by Maverick
	JE extraction_done		;


	mov	eax,[esi+18h]
	movzx	ecx,word [esi+20h]
	lea	esi,[buffer+eax]	; pointer to section relocations
	mov	edi,esi
	mov	ebx,edi
    convert_relocations:
	lodsd
	stosd
	lodsd
	mov	edx,12h
	mul	edx
	add	eax,dword [buffer+8]
	add	eax,buffer
	cmp	[eax+0Ch],bp
	jne	unsupported_relocation
	cmp	dword [eax+8],0
	jne	unsupported_relocation
	lodsw
	cmp	ax,6
	jne	unsupported_relocation
	loop	convert_relocations
	mov	eax,0xFFFFFFFF		; Added by Dex
	stosd				;
	sub	edi,ebx
	invoke	WriteFile,[output2],ebx,edi,bytes_count,0
	jmp	extraction_done

    unsupported_relocation:
	call	error
	db	"No relocations found.",0Dh,0Ah,0
       ;db      "Unsupported relocation type found.",0Dh,0Ah,0

    next_section:
	add	esi,28h
	cmp	eax,ecx
	jb	find_flat_section
	call	error
	db	"Source file doesn't contain .flat section.",0

invalid_format:
	call	error
	db	"Source file is not COFF.",0Dh,0Ah,0

extraction_done:
