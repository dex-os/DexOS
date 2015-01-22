 ;----------------------------------------------------;
 ; Sets the text pos                                  ;
 ;----------------------------------------------------;

set_text_pos:
	mov   [screen_x],al
	mov   [screen_y],ah
	ret

 ;----------------------------------------------------;
 ; TextColor                        ; Sets text color ;
 ;----------------------------------------------------;

TextColor2:
	mov   byte[text_color],al
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints char               ; print's a char from al ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      Al Ascii character to write.                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;

print_char:
	pushad
	push  es
	push  ds
	cli
	push  ax
	mov   ax,18h
	mov   es,ax			     
	pop   ax	
	mov   edi,FasmScreenBuffer
	call  get_scr_pointer
	mov   ah,[text_color]
	mov   [es:edi],ax		     
	call  inc_scr_pointer
	sti
	pop   ds
	pop   es
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints string      ; does what it say's on the box ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      es:esi points to asciiz string to write.      ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
print_string:
	pushad					 
	push  es
	push  ds
aloop:
	mov   al,[esi]
	cmp   al,0					   ; 0x00 = end of string
	je    gend
	cmp   al,0xd2					   ; 0xd2 = next byte is color byte
	je    ColorChange
	cmp   al,0x0d					   ; 0x0d = CR ( = \n )
	jne   no_cr
	call  carriage_return
	jmp   a1
ColorChange:
	inc   esi
	mov   al,[esi]
	mov   byte[text_color],al
	inc   esi
	jmp   aloop
no_cr:
	call  print_char
a1:	inc   esi
	jmp   aloop
gend:
	pop   ds
	pop   es
	popad
	ret

 ;----------------------------------------------------;
 ; Carriage return                       ; next line  ;
 ;----------------------------------------------------;
carriage_return:
	push  eax
	mov   [screen_x],byte 0
	mov   ah,[screen_y]	
	cmp   ah,100 ;24
	jb    no_scroll
	jmp   cend
no_scroll:
	inc   ah    
	mov   [screen_y],ah	
cend:	pop   eax
	ret


 ;----------------------------------------------------;
 ; Inc screen pointer                                 ;
 ;----------------------------------------------------;
inc_scr_pointer:
	push  eax
	cld
	mov   al,[screen_x]
	cmp   al,69 ;49 ;79
	jb    no_incy
	call  carriage_return
	jmp   dend
no_incy:
	inc   al
	mov   [screen_x],al
dend:	pop   eax
	ret

 ;----------------------------------------------------;
 ; Gets screen pointer                                ;
 ;----------------------------------------------------;
get_scr_pointer:
	push  eax					   
	push  ebx
	xor   ebx,ebx
	mov   bl,[screen_x]
	shl   ebx,1
	add   edi,ebx
	mov   bl,[screen_y]
	mov   eax,140 ;0x64 ;0xa0
	mul   bx
	add   edi,eax
	pop   ebx
	pop   eax
	ret

 ;----------------------------------------------------;
 ; Data.                                              ;
 ;----------------------------------------------------;

screen_x      db 0
screen_y      db 0
text_color    db 0x78

