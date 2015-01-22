
;=======================================================  ;
; Program  Entry                                          ;
;=======================================================  ;
ViewText:						  ;
	       pushad					  ;
	       mov	ax,18h				  ;
	       mov	ds,ax				  ;
	       mov	es,ax				  ;
	       mov	ebx,ImageLoadAddrVar		  ;
	       mov	eax,out_buffer			  ; put it in 2nd counter
	       xor	ecx,ecx 			  ; 0 line position
	       xor	edi,edi 			  ;
;=======================================================  ;
; process.                                                ;
;=======================================================  ;
process:						  ;
	       mov	dl,[ebx]			  ; get char
	       cmp	dl,09h				  ; is it tab?
	       je	new_tabber			  ;
	       cmp	dl,0Ah				  ; is it linefeed?
	       je	println 			  ;
	       cmp	dl, 0Dh 			  ; is it cariage return?
	       je	println 			  ; print the line
	       cmp	dl, 1Ah 			  ; is it end of file?
	       je	end_of_file			  ;
	       cmp	dl, 00h 			  ; is it null?
	       je	end_of_file			  ;
	       mov	[eax], dl			  ; if not, stick in buffer
	       cmp	eax, out_buffer + 80		  ; check buffer, full?
	       jae	println 			  ; yeah, print the buffer
	       inc	eax				  ; else next buffer offset
	       inc	ebx				  ; next  char
	       inc	ecx				  ; scrn pos inc
	       jmp	process 			  ; loop
;=======================================================  ;
; nothing.                                                ;
;=======================================================  ;
nothing:						  ;
	       inc	ebx				  ;
	       jmp	process 			  ;
;=======================================================  ;
; new_tabber.                                             ;
;=======================================================  ;
new_tabber:						  ;
	       mov	esi, ecx			  ; save the eax
	       and	cl, 7				  ; get the desired area
	       cmp	cl, 0				  ; chk for 0
	       jz	simp_tab			  ; if 0 simple  tab
	@@:    inc	esi				  ; incrememt esi
	       inc	eax				  ; increment eax
	       mov	ecx, esi			  ; reload ecx
	       and	cl, 7				  ; get desired area
	       cmp	cl, 0				  ; check if 0
	       jz	@f				  ; if yeah keep spot
	       jmp	@b				  ; if not, another round
	@@:						  ;
	       mov	ecx, esi			  ;
	       inc	ebx				  ;
	       jmp	process 			  ;
;=======================================================  ;
; simp_tab                                                ;
;=======================================================  ;
      simp_tab: 					  ;
	       mov	ecx, esi			  ;
	       add	eax, 8				  ;
	       add	ecx, 8				  ;
	       inc	ebx				  ;
	       jmp	process 			  ;
;=======================================================  ;
; end_of_file                                             ;
;=======================================================  ;
end_of_file:						  ;
	       mov	byte [eax], 0			  ; check for null
	       mov	esi,out_buffer			  ;
	       call	[PrintString_0] 		  ;
	       xor	eax, eax			  ; 0 counters
	       xor	ebx, ebx			  ;
	       xor	ecx, ecx			  ;
	       mov	esi,Space
	       call	[PrintString_0] 		  ;
;=======================================================  ;
; wait for keypress                                       ;
;=======================================================  ;
	       popad					  ;
	       ret					  ; end of prog.
;=======================================================  ;
; println                                                 ;
;=======================================================  ;
println:						  ;
		cmp	dl, 0dh 			  ; is it <cr>?
		jnz	@f				  ; if not, continue
		inc	ebx				  ; else get next char
		cmp	byte [ebx], 0ah 		  ; is it <lf>?
		jz	@f				  ; if so coninue on 1 return
		dec	ebx				  ; else reinstate old pointer
	 @@:						  ;
		mov	byte [eax], 13			  ;
		inc	eax				  ;
		mov	byte [eax], 10			  ;
		inc	eax				  ;
		mov	byte [eax], 0			  ; stick in the null char
		push	esi				  ;
		mov	esi,out_buffer			  ;
		call	[PrintString_0] 		  ;
		pop	esi				  ;
		mov	eax, out_buffer 		  ; put address in eax
		mov	ecx, 80 			  ;
	 @@:						  ;
		mov	byte [eax],' '			  ; make buffer spaces
		inc	eax				  ;
		dec	ecx				  ;
		jnz	@b				  ;
		mov	eax, out_buffer 		  ; reset eax to buffer 0
		inc	ebx				  ; tell next char
		xor	ecx,ecx 			  ; 0 the screen index
		inc	edi				  ;
		cmp	edi, 23 			  ; is bottom of screen?
		jnz	process 			  ; if not, keep going
		push	eax				  ; save eax
		push	esi				  ;
		mov	esi,message			  ;
		call	[PrintString_0] 		  ;
		pop	esi				  ;
		call	[WaitForKeyPress]		  ;
		xor	edi, edi			  ;
		cmp	al,0x1b 			  ;
		je	TextViewExit			  ;
		pop	eax				  ;
		jmp	process 			  ; loop
;=======================================================  ;
; TextViewExit                                            ;
;=======================================================  ;
TextViewExit:						  ;
		pop	eax				  ;
		popad					  ;
		ret					  ;

Space:		     db  0xd2,0x09,13,'End of file ',0xd2,0x07,13,10,0
message:	     db 0xd2,0x09,13,'Press a anykey to show more or esc for exit.',0xd2,0x07,13,13,10,0

out_buffer:
	times 90 db ' '
		  dd 0
