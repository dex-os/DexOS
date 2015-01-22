
; flat assembler interface for DexOS
; Copyright (c) 1999-2011, Tomasz Grysztar.
; All rights reserved.

Fasm:
	pushad
	push	es
	push	ds
	cli						  
	mov	ebp,esp 			       
	mov	[return],ebp			       
	mov	esp,Stack1		       
	sti			
	mov	ax,18h
	mov	ds,ax
	mov	es,ax
	mov	[AdressAddOn],0
	mov	[screen_x],0
	mov	[screen_y],0
	mov	edi,FasmScreenBuffer
	mov	ecx,100*80
	mov	al,' '
	mov	ah,color2a
	rep	stosw

	mov	esi,[Params]
	mov	dword[command_line],esi

	mov	esi,_logo	
	call	print_string

	call	get_params   
	jc	information

	call	init_memory 
	mov	esi,_memory_prefix
	call	print_string
	mov	eax,[memory_end]
	sub	eax,[memory_start]
	add	eax,[additional_memory_end]
	sub	eax,[additional_memory]
	shr	eax,10
	call	display_number
	mov	esi,_memory_suffix
	call	print_string
       ; xor     ah,ah
       ; int     1Ah
       ; mov     ax,cx
       ; shl     eax,16
       ; mov     ax,dx
       ; mov     [start_time],eax
	call	preprocessor
	call	parser
	call	assembler
	call	formatter
	call	display_user_messages
	movzx	eax,[current_pass]
	inc	eax
	call	display_number
	mov	esi,_passes_suffix
	call	print_string
	jmp	display_bytes_count		    
	call	[TimerSecondCount]
	sub	eax,[start_time]
	mov	ebx,100
	mul	ebx
	mov	ebx,182
	div	ebx
	or	eax,eax
	jz	display_bytes_count
	xor	edx,edx
	mov	ebx,10
	div	ebx
	push	edx
	call	display_number
	mov	al,'.'
	call	display_character1
	pop	eax
	call	display_number
	mov	esi,_seconds_suffix
	call	print_string
display_bytes_count:
	mov	eax,[written_size]
	call	display_number
	mov	esi,_bytes_suffix
	call	print_string
	xor	al,al
	jmp	exit_program

;----------------------------------------------------;
; information.                                       ;
;----------------------------------------------------;
information:
	mov	esi,_usage
	call	print_string
	mov	al,1
	jmp	exit_program

;----------------------------------------------------;
; get_params.                                        ;
;----------------------------------------------------;
get_params:
	mov	[input_file],0
	mov	[output_file],0
	mov	[symbols_file],0
	mov	[memory_setting],0
	mov	[passes_limit],100
	mov	esi,dword[command_line]
	mov	edi,params
    find_param:
	lodsb
	cmp	al,20h
	je	find_param
	cmp	al,'-'
	je	option_param
	cmp	al,0Dh
	je	all_params
	or	al,al
	jz	all_params
	cmp	[es:input_file],0
	jne	get_output_file
	mov	[es:input_file],edi
	jmp	process_param
      get_output_file:
	cmp	[es:output_file],0
	jne	bad_params
	mov	[es:output_file],edi
    process_param:
	cmp	al,22h
	je	string_param
    copy_param:
	stosb
	lodsb
	cmp	al,20h
	je	param_end
	cmp	al,0Dh
	je	param_end
	or	al,al
	jz	param_end
	jmp	copy_param
    string_param:
	lodsb
	cmp	al,22h
	je	string_param_end
	cmp	al,0Dh
	je	param_end
	or	al,al
	jz	param_end
	stosb
	jmp	string_param
    option_param:
	lodsb
	cmp	al,'m'
	je	memory_option
	cmp	al,'M'
	je	memory_option
	cmp	al,'p'
	je	passes_option
	cmp	al,'P'
	je	passes_option
	cmp	al,'s'
	je	symbols_option
	cmp	al,'S'
	je	symbols_option
    invalid_option:
	stc
	ret
    get_option_value:
	xor	eax,eax
	mov	edx,eax
    get_option_digit:
	lodsb
	cmp	al,20h
	je	option_value_ok
	cmp	al,0Dh
	je	option_value_ok
	or	al,al
	jz	option_value_ok
	sub	al,30h
	jc	bad_params_value
	cmp	al,9
	ja	bad_params_value
	imul	edx,10
	jo	bad_params_value
	add	edx,eax
	jc	bad_params_value
	jmp	get_option_digit
    option_value_ok:
	dec	esi
	clc
	ret
    bad_params_value:
	stc
	ret
    memory_option:
	lodsb
	cmp	al,20h
	je	memory_option
	cmp	al,0Dh
	je	invalid_option
	or	al,al
	jz	invalid_option
	dec	esi
	call	get_option_value
	jc	invalid_option
	or	edx,edx
	jz	invalid_option
	cmp	edx,1 shl (32-10)
	jae	invalid_option
	mov	[es:memory_setting],edx
	jmp	find_param
    passes_option:
	lodsb
	cmp	al,20h
	je	passes_option
	cmp	al,0Dh
	je	invalid_option
	or	al,al
	jz	invalid_option
	dec	esi
	call	get_option_value
	jc	bad_params
	or	edx,edx
	jz	invalid_option
	cmp	edx,10000h
	ja	invalid_option
	mov	[es:passes_limit],dx
	jmp	find_param
    symbols_option:
	mov	[es:symbols_file],edi
      find_symbols_file_name:
	lodsb
	cmp	al,20h
	jne	process_param
	jmp	find_symbols_file_name
    param_end:
	dec	esi
    string_param_end:
	xor	al,al
	stosb
	jmp	find_param
    all_params:
	xor	al,al
	stosb

	cmp	[input_file],0
	je	bad_params
	clc
	ret
    bad_params:
	stc
	ret

include 'system.inc'

include '..\version.inc'
ExitMesage:	  db   13,13,'Press any key to exit fasm',0
_copyright db 'Copyright (c) 1999-2010, Tomasz Grysztar',13,0

_logo db 'flat assembler  version ',VERSION_STRING,0
_usage db 13
       db 'usage: fasm <source> [output]',13
       db 'optional settings:',13
       db ' -m <limit>    set the limit in kilobytes for the available memory',13
       db ' -p <limit>    set the maximum allowed number of passes',13
       db ' -s <file>     dump symbolic information for debugging',13
       db 0
_memory_prefix db '  (',0
_memory_suffix db ' kilobytes memory)',13,0
_passes_suffix db ' passes, ',0
_seconds_suffix db ' seconds, ',0
_bytes_suffix db ' bytes.',13,0

include '..\errors.inc'
include '..\symbdump.inc'
include '..\preproce.inc'
include '..\parser.inc'
include '..\exprpars.inc'
include '..\assemble.inc'
include '..\exprcalc.inc'
include '..\formats.inc'
include '..\x86_64.inc'
include '..\avx.inc'

include '..\tables.inc'
include '..\messages.inc'

align 4

include '..\variable.inc'

return dd ?
command_line dd ?
memory_setting dd ?
environment dd ?
timestamp dq ?
start_time dd ?
displayed_count dd ?
last_displayed db ?
character db ?
params rb 100h
;include 'Dex.inc'               ; Here is where we includ our "DexFunctions.inc" file
Stack2:  rb 1024*100
Stack1: 			 ; remember the stack works backward
include 'Filehand.inc'			 ; remember the stack works backward
buffer44 rb 1000h
