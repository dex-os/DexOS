
; flat assembler interface for DosExtreme
; Copyright (c) 1999-2010, Tomasz Grysztar.
; All rights reserved.
LoadAddress = 0xa00000 + 0x1200000
filepos 	 dd 0x0
;====================================================;
; init_memory.                                *fasm* ;
;====================================================;
init_memory:
	mov	eax,esp
       ; and     eax,not 0FFFh
	add	eax,1000h-10000h
	mov	[stack_limit],eax
	mov	eax,0xd00000 + 0x1200000
	mov	[additional_memory],eax
	mov	ecx,[memory_setting]
	shl	ecx,10
	jnz	allocate_memory
	mov	ecx,1000000h
      allocate_memory:
	mov	ebx,[additional_memory]
	add	ebx,ecx
	mov	eax,ebx
	mov	[memory_end],eax
	sub	eax,[additional_memory]
	shr	eax,2
	add	eax,[additional_memory]
	mov	[additional_memory_end],eax
	mov	[memory_start],eax
	ret

;====================================================;
; exit_program.                               *fasm* ;
;====================================================;
exit_program:
	mov	ebp,[return]
	mov	esp,ebp
	ret

;====================================================;
; get_environment_variable.                   *fasm* ;
;====================================================; 
get_environment_variable:
;Code will be added for "environment variable" later.;

	ret

;====================================================;
; make_timestamp.                             *fasm* ;
;====================================================;
make_timestamp:
;Code will be added for "timestamp" later.;
	
	ret

;====================================================;
; open.                                       *fasm* ;
; in                                                 ;
;    edx = zero-ended file name                      ;
;                                                    ;
; out                                                ;
;     ebx = file handle                              ;
;====================================================;
tempHandler  dd 0
FileSize_Handler dd 0
;====================================================;
; open.                                       *fasm* ;
;====================================================; 
open:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   dword[tempHandler],0
	mov   [FileNameVar],edx
	mov   eax,9					; Open file
	call  [Fatmod]
	jc    file_error
	xor   ebx,ebx
	mov   bx,ax
	mov   dword[tempHandler],ebx
	mov   dword[FileSize_Handler],ecx
	mov   esi,edx
	mov   edi,Filehandle
	mov   ecx,20
	xor   eax,eax
FileHloop:
	add   edi,32 
	inc   ax
	cmp   byte[es:edi],0
	je    Found
	loop  FileHloop
	jmp   file_error
Found:
	mov   [FileHandleNumer],eax
	mov   esi,[FileNameVar]
	mov   [testfile],edi
@@:
	lodsb
	stosb
	mov   ebx,dword[tempHandler]
	mov   edx,LoadAddress
	add   edx,[AdressAddOn]
	mov   [LoadAdressOfFile],edx
	mov   eax,11					; Read file
	call  [Fatmod]
	jc    file_error_H
	mov   [filesize], eax				;;;;;;;;;;;;;;;;;;;;;
	add   [AdressAddOn],eax
	add   [AdressAddOn],512
	mov   edi,[testfile]
	add   edi,12
	mov   eax,[LoadAdressOfFile]
	stosd
	mov   eax,[filesize]
	stosd
	mov   ebx,dword[tempHandler]
	mov   eax,10					; Close file
	call  [Fatmod]
	pop   ds
	pop   es
	popad
	mov  ebx,[FileHandleNumer]
	clc
	ret

file_error:
	mov   [ErrorCode],al
	pop   ds
	pop   es
	popad
	mov   ah,[ErrorCode]
	stc
	ret

file_error_H:
	push  eax
	mov   ebx,dword[tempHandler]
	mov   eax,10					; Close file
	call  [Fatmod]
	pop   eax
	mov   [ErrorCode],al
	pop   ds
	pop   es
	popad
	mov   ah,[ErrorCode]
	stc
	ret

;====================================================;
; read.                                       *fasm* ;
;====================================================; 
read:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   dword[NumberOfBinFile],ecx
	mov   dword[Buffer],edx
	mov   dword[Fhandler],ebx
	mov   edi,dword[Buffer]
	call  GetPointer
	add   esi,[filepos]
	mov   ecx,[NumberOfBinFile]
	cld
	rep   movsb
	pop   ds
	pop   es
	popad
	clc
	ret

BufferAA dd 0
; ebx file handle
; ecx count of bytes to write
; edx pointer to buffer
;====================================================;
; write.                                      *fasm* ;
;====================================================; 
write:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   dword[NumberOfBinFile],ecx
	mov   dword[BufferAA],edx
	mov   dword[Fhandler],ebx
	cmp   [Fhandler],20
	ja    WriteFileToFloppyError
	cmp   [Fhandler],0
	je    WriteFileToFloppyError
	mov   esi,Filehandle
	mov   edx,[Fhandler]
	shl   edx,5  
	add   esi,edx
	mov   [NameAddress],esi
	mov   edx,esi
	mov   eax,12					; Delete file
	call  [Fatmod]
	mov   eax,8
	mov   ebx,[NameAddress]
	mov   edx,dword[BufferAA]
	mov   ecx,[NumberOfBinFile]
	call  [Fatmod]
	jc    WriteFileToFloppyError
@@:
	pop   ds
	pop   es
	popad
	clc
	ret	

WriteFileToFloppyError:
	pop   ds
	pop   es
	popad
	stc
	ret		


;====================================================;
; close.                                      *fasm* ;
;====================================================; 
close:	
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   edi,Filehandle
	shl   ebx,5  
	add   edi,ebx
	mov   ecx,30
	mov   al,0
	rep   stosb
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; lseek.                                      *fasm* ;
;====================================================; 
lseek:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   [Fhandler],ebx
	call  GetFileSize
	mov   [filesize],esi
	cmp   al,0
	jnz   @f
	mov   [filepos],0
@@:	cmp   al,1
	jnz   @f
@@:	cmp   al,2
	jnz   @f
	mov   eax,[filesize]
	mov   [filepos],eax
@@:	mov   eax,[filepos]
	add   eax,edx
	mov   [filepos],eax
	pop   ds
	pop   es
	popad
	mov   eax,[filepos]
	ret


;====================================================;
; GetPointer.                                        ;
;====================================================; 
GetPointer:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   esi,Filehandle
	mov   edx,[Fhandler]
	shl   edx,5  
	add   esi,edx
	add   esi,12
	lodsd
	mov   [var],eax
	pop   ds
	pop   es
	popad
	mov   esi,[var]
	ret

;====================================================;
; GetFileSize.                                       ;
;====================================================; 
GetFileSize:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   esi,Filehandle
	mov   edx,[Fhandler]
	shl   edx,5  
	add   esi,edx
	add   esi,16
	lodsd
	mov   [var],eax
	pop   ds
	pop   es
	popad
	mov   esi,[var]
	ret

;====================================================;
; create.                                     *fasm* ;
;====================================================; 
create:
	pushad
	push  es
	push  ds
	push  eax
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	pop   eax
	mov   [FileNameVar],edx
	mov   edi,Filehandle
	mov   ecx,20			    ;;;;;;;;;;;;;;;
	xor   eax,eax
FileHcreateloop:
	add   edi,32 
	inc   ax
	cmp   byte[es:edi],0
	je    FoundCreate
	loop  FileHcreateloop
	jmp   CreateError
FoundCreate:  
	mov   [FileHandleNumer],eax
	mov   esi,[FileNameVar]
	mov   [testfile],edi
@@:
	lodsb
	stosb
	cmp   al,0
	jne   @b
	pop   ds
	pop   es
	popad
	mov   ebx,[FileHandleNumer]
	clc    
	ret

CreateError:
	pop   ds
	pop   es
	popad
	stc
	ret


;====================================================;
; display_string.                             *fasm* ;
;====================================================; 
display_string:
	call	[PrintString_0]
      string_end:
	ret

;====================================================;
; display_number.                             *fasm* ;
;====================================================;
display_number:
	push	ebx
	mov	ecx,1000000000
	xor	edx,edx
	xor	bl,bl
display_loop:
	div	ecx
	push	edx
	cmp	ecx,1
	je	display_digit
	or	bl,bl
	jnz	display_digit
	or	al,al
	jz	digit_ok
	not	bl
display_digit:
	add	al,30h
	call	[PrintChar]
digit_ok:
	mov	eax,ecx
	xor	edx,edx
	mov	ecx,10
	div	ecx
	mov	ecx,eax
	pop	eax
	or	ecx,ecx
	jnz	display_loop
	pop	ebx
	ret
;====================================================;
; display_user_messages                       *fasm* ;
;====================================================;
display_user_messages:
	mov	[displayed_count],0
	call	show_display_buffer
	cmp	[displayed_count],1
	jb	line_break_ok
	je	make_line_break
	mov	ax,word [last_displayed]
	cmp	ax,0A0Dh
	je	line_break_ok
	cmp	ax,0D0Ah
	je	line_break_ok
      make_line_break:
	mov	esi,cr_lf
	call	[PrintString_$]
      line_break_ok:
	ret
;====================================================;
; display_block.                              *fasm* ;
;====================================================; 
display_block:
	jecxz	block_displayed
	add	[displayed_count],ecx
	mov	al,[esi+ecx-1]
	mov	[last_displayed],al
	push	ebx
DisplayCharacter:
	lods	byte [esi]
	call	DisplayChar
	loopd	DisplayCharacter
	pop	ebx
block_displayed:
	ret

;====================================================;
; display_character.                          *fasm* ;
;====================================================; 
display_character:
	lods	byte [esi]
	call	DisplayChar  
	loopd	display_character
	ret
;====================================================;
; fatal_error.                                *fasm* ;
;====================================================;  
fatal_error:
	mov	esi,error_prefix
	call	display_string
	pop	esi
	call	display_string
	mov	esi,error_suffix
	call	display_string
	mov	al,0FFh
	jmp	exit_program
;====================================================;
; assembler_error.                            *fasm* ;
;====================================================;  
assembler_error:
	call	display_user_messages
	push	dword 0
	mov	ebx,[current_line]
get_error_lines:
	push	ebx
	test	byte [ebx+7],80h
	jz	display_error_line
	mov	edx,ebx
find_definition_origin:
	mov	edx,[edx+12]
	test	byte [edx+7],80h
	jnz	find_definition_origin
	push	edx
	mov	ebx,[ebx+8]
	jmp	get_error_lines
display_error_line:
	mov	esi,[ebx]
	call	display_string
	mov	esi,line_number_start
	call	display_string
	mov	eax,[ebx+4]
	and	eax,7FFFFFFFh
	call	display_number
	mov	al,']'
	call	display_character1 
	pop	esi
	cmp	ebx,esi
	je	line_number_ok
	mov	al,20h
	call	display_character1 
	push	esi
	mov	esi,[esi]
	movzx	ecx,byte [esi]
	inc	esi
	call	display_block
	mov	esi,line_number_start
	call	display_string
	pop	esi
	mov	eax,[esi+4]
	and	eax,7FFFFFFFh
	call	display_number
	mov	al,']'
	call	display_character1 
      line_number_ok:
	mov	esi,line_data_start
	call	display_string
	mov	esi,ebx
	mov	edx,[esi]
	call	open
	mov	al,2
	xor	edx,edx
	call	lseek
	mov	edx,[esi+8]
	sub	eax,edx
	push	eax
	xor	al,al
	call	lseek
	mov	ecx,[esp]
	mov	edx,[additional_memory]
	lea	eax,[edx+ecx]
	cmp	eax,[additional_memory_end]
	ja	out_of_memory
	call	read
	call	close
	pop	ecx
	mov	esi,[additional_memory]
get_line_data:
	mov	al,[esi]
	cmp	al,0Ah
	je	display_line_data
	cmp	al,0Dh
	je	display_line_data
	cmp	al,1Ah
	je	display_line_data
	or	al,al
	jz	display_line_data
	inc	esi
	loop	get_line_data
display_line_data:
	mov	ecx,esi
	mov	esi,[additional_memory]
	sub	ecx,esi
	call	display_block
	mov	esi,cr_lf
	call	display_string
	pop	ebx
	or	ebx,ebx
	jnz	display_error_line
	mov	esi,error_prefix
	call	display_string
	pop	esi
	call	display_string
	mov	esi,error_suffix
	call	display_string
	jmp	exit_program

;====================================================;
; DisplayChar.                                       ;
;====================================================; 
DisplayChar:
	pushad
	cmp	al,13
	jne	@f
	call	Next_line
	jmp	DisplayCharEnd
@@:
	cmp	al,0x20
	jne	@f
	call	space
	jmp	DisplayCharEnd
@@:
	cmp	al,0x09
	jne	@f
	call	tab
	jmp	DisplayCharEnd
@@:
	call	[PrintChar]
DisplayCharEnd:
	popad
	ret

;====================================================;
; display_character1.                                ;
;====================================================; 
display_character1:
	call	[PrintChar] 
	ret

;====================================================;
; next line.                                         ;
;====================================================; 
Next_line:
	push	esi
	mov	esi,cr_lf
	call	display_string
	pop	esi
	ret
      
;====================================================;
; space.                                             ;
;====================================================; 
space:
	push	esi
	mov	esi,space1
	call	display_string
	pop	esi
	ret   

;====================================================;
; tab.                                               ;
;====================================================; 
tab:
	push	esi
	mov	esi,tab1
	call	display_string
	pop	esi
	ret
;====================================================;
; Just in case no fat module loaded                  ;
;====================================================;
NoModuleLoaded:
	stc
	ret

;====================================================;
; Data.                                              ;
;====================================================;
Fatmod		  dd  NoModuleLoaded
ID		  db 'DEXOSFAT',0
var		  dd 0
LoadAdressOfFile  dd 0
testfile	  dd 0
NameAddress	  dd 0
AdressAddOn	  dd 0
AmountOfBytes	  dd 0
NumberOfBinFile   dd 0
Buffer		  dd 0
Fhandler	  dd 0
FileHandleNumer   dd 0
ErrorCode	  db 0
filesize	  dd 0x0
FileNameVar	  dd 0
error_prefix	  db 'error: ',0
error_suffix	  db '.',13,10,0
line_data_start   db ':',13,10,0
cr_lf		  db 13,10,0
line_number_start db ' [',0
space1		  db ' ',0
tab1		  db '      ',0
macro dm string { db string,0 }

