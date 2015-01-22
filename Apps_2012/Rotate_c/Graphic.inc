
 ;----------------------------------------------------;
 ; copy_buffer.                                       ;
 ;----------------------------------------------------;
copy_buffer:
	push  ds
	push  es
	mov   esi,buffer_seg1
	mov   edi,0xa0000
	mov   ax,08h
	mov   es,ax
	mov   ecx,64000/8
cb_copy:
	fld   qword  [ds:esi]
	add   esi,8
	fstp  qword[es:edi]
	add   edi,8
	dec   ecx
	jnz   cb_copy
	pop   ds
	pop   es
	ret

 ;----------------------------------------------------;
 ; retrace.                                           ;
 ;----------------------------------------------------;
retrace:
	mov   dx,03dah
c1:
	in    al,dx
	test  al,8
	jz    c1
c2:
	in    al,dx
	test  al,8
	jnz   c2
	ret


 ;----------------------------------------------------;
 ; Check valid bmp.                                   ;
 ;----------------------------------------------------;
DisplayBMP:
	mov   esi,LoadMeHere
	cmp   word [ds:esi],'BM'	
	jnz   bmp_error
	mov   bx,[ds:esi + 18]
	mov   ebp,[ds:esi + 22]
	cmp   bx,320
	ja    bmp_error
	cmp   ebp,200
	ja    bmp_error
	cmp   word [ds:esi + 28],8
	jnz   bmp_error

	mov   ax,0x0013
	call  [RealModeInt10h]		    

 ;----------------------------------------------------;
 ; Set pal.                                           ;
 ;----------------------------------------------------;
	add   esi,0x36
	mov   cx,256
	mov   dx,0x03c8
	mov   al,0
	out   dx,al
	inc   dx

set_pal:
	mov   al,[esi+2]
	shr   al,2
	out   dx,al
	mov   al,[esi+1]
	shr   al,2
	out   dx,al
	mov   al,[esi]
	shr   al,2
	out   dx,al
	add   esi,4
	loop  set_pal

	lea   dx,[bx+3]
	and   dx,-4
	imul  edi,ebp,320
	add   edi,ImageBuffer
new_line:
	sub   edi,320
	pushad
	mov   cx,bx
	rep   movsb
	popad
	add   esi,edx
	dec   ebp
	jnz   new_line
	jmp   @f


 ;----------------------------------------------------;
 ; Error message.                                     ;
 ;----------------------------------------------------;
bmp_error:
	mov   esi,msg1
	call  [PrintString_0]

 ;----------------------------------------------------;
 ; wait for key press                                 ;
 ;----------------------------------------------------;
@@:
	ret

 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;
FileName	dd  0
LetterCount	dw  0


