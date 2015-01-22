
format PE console
entry start

include 'include\kernel.inc'

include 'include\macro\stdcall.inc'
include 'include\macro\import.inc'

section '.code' code readable executable

start:

	invoke	GetCommandLine
	mov	esi,eax
	mov	edi,params
    find_command_start:
	lodsb
	cmp	al,20h
	je	find_command_start
	cmp	al,22h
	je	skip_quoted_name
    skip_name:
	lodsb
	cmp	al,20h
	je	find_param
	or	al,al
	jz	all_params
	jmp	skip_name
    skip_quoted_name:
	lodsb
	cmp	al,22h
	je	find_param
	or	al,al
	jz	all_params
	jmp	skip_quoted_name
    find_param:
	lodsb
	cmp	al,20h
	je	find_param
	cmp	al,22h
	je	string_param
	cmp	al,0Dh
	je	all_params
	or	al,al
	jz	all_params
	inc	edi
	mov	ebx,edi
    copy_param:
	stosb
	lodsb
	cmp	al,20h
	je	param_end
	or	al,al
	jz	param_end
	jmp	copy_param
    string_param:
	inc	edi
	mov	ebx,edi
    copy_string_param:
	lodsb
	cmp	al,22h
	je	string_param_end
	or	al,al
	jz	param_end
	stosb
	jmp	copy_string_param
    param_end:
	dec	esi
    string_param_end:
	xor	al,al
	stosb
	mov	eax,edi
	sub	eax,ebx
	mov	[ebx-1],al
	jmp	find_param
    all_params:
	xor	al,al
	stosb

	cmp	[params],0
	je	information
	lea	eax,[params+1]
	mov	[input],eax
	movzx	ecx,byte [eax-1]
	add	eax,ecx
	cmp	byte [eax],0
	je	information
	inc	eax
	mov	[output1],eax
	movzx	ecx,byte [eax-1]
	add	eax,ecx
	cmp	byte [eax],0
	je	information
	inc	eax
	mov	[output2],eax

	invoke	CreateFile,[input],GENERIC_READ,0,0,OPEN_EXISTING,0,0
	mov	[input],eax
	inc	eax
	jz	file_not_found
	invoke	CreateFile,[output1],GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0
	mov	[output1],eax
	invoke	CreateFile,[output2],GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0
	mov	[output2],eax

	invoke	GetFileSize,[input],NULL
	invoke	ReadFile,[input],buffer,eax,bytes_count,0
	invoke	CloseHandle,[input]

	include 'Coff2Rel.ASM'

	invoke	CloseHandle,[output1]
	invoke	CloseHandle,[output2]
	cmp	[No2CUT_Found],1
	je	Warning
	invoke	ExitProcess,0

display_string:
	push	esi
	push	STD_OUTPUT_HANDLE
	call	[GetStdHandle]
	mov	ebp,eax
	pop	esi
	mov	edi,esi
	or	ecx,-1
	xor	al,al
	repne	scasb
	neg	ecx
	sub	ecx,2
	push	0
	push	bytes_count
	push	ecx
	push	esi
	push	ebp
	call	[WriteFile]
	ret

;-
Warning:
	mov	esi,_WarningMes
	call	display_string
	invoke	ExitProcess,1
;-
information:
	mov	esi,_info
	call	display_string
	invoke	ExitProcess,1
file_not_found:
	call	error
	db	"source file not found",0Dh,0Ah,0
error:
	pop	esi
	call	display_string
	invoke	ExitProcess,2

section '.data' data readable writeable
_WarningMes  db "Warning no 2CUT found",0Dh,0Ah
	     db 0

_info	     db "flat COFF extractor",0Dh,0Ah
	     db "usage: COFF2DEX input_coff output_code output_reloc",0Dh,0Ah
	     db 0

No2CUT_Found db 0 ;cc
input dd ?
output1 dd ?
output2 dd ?
bytes_count dd ?
params rb 1000h
buffer rb 2000000 ;680000h     ;180000h

section '.idata' import data readable writeable

  library kernel,'KERNEL32.DLL',\
	  user,'USER32.DLL'

  kernel:
  import GetCommandLine,'GetCommandLineA',\
	 GetStdHandle,'GetStdHandle',\
	 CreateFile,'CreateFileA',\
	 ReadFile,'ReadFile',\
	 WriteFile,'WriteFile',\
	 GetFileSize,'GetFileSize',\
	 CloseHandle,'CloseHandle',\
	 ExitProcess,'ExitProcess'

  user:
  import MessageBox,'MessageBoxA'

section '.reloc' fixups data readable discardable
