;==========================================================================
; JPEG decompresser function.
;--------------------------------------------------------------------------
;
; INPUT:
;           ESI = Compressed Image Buffer (Source buffer)
;           EDI = Uncompressed Image Buffer (Dest buffer)
;           ECX = Size of file (Compressed)
;
; EXIT:
;           All regs saved
;
; Returns:
;           ReSized Image Buffer + 0 = X
;           ReSized Image Buffer + 4 = Y
;           ReSized Image Buffer + 8 = Uncompressed image (32bpp format)
;
; ERROR:    Sets CF to 1
;
; VERY IMPORTANT: 
;           THE JPEG MUST BE A MULTPLY OF 16, EG: 16, 32, 48 etc
;==========================================================================
DeCodeJpeg:
	pushad
	mov   dword[JpegInput],esi
	mov   dword[JpegOutput],edi
	add   dword[JpegOutput],8
	mov   dword[JpegSize],ecx
;;;;;;;;;
        mov   esi,iniciomemoria
        mov   edi,iniciomemoria1
        mov   ecx,6
        rep   movsd

        mov   edi,iniciomemoriaPacker
        mov   al,0
        mov   ecx,100
        rep   stosb
;;;;;;;;;
	mov   word[Xsize],0
	mov   word[Ysize],0
	mov   word[YsizeImage],0      ; Add by C
	mov   ecx,200000h	      ; Add by C cccccccccc
	mov   edi,fin
	call  add_mem
	mov   ecx,16384
	call  malloc
        jc    DeCodeJpegError
	mov   [work_area],edi
	call  colorprecalc
	mov   esi, dword[JpegInput]
	mov   edi, dword[JpegOutput]
	mov   ecx, dword[JpegSize]
	call  open
	call  jpeg_info
	test  ebp,ebp
	jz    DeCodeJpegError
	mov   [JpegStart],ebp
	push  edi
	push  eax
	mov   edi,dword [JpegStart]
	xor   eax,eax
	mov   ax,word[edi+x_size]
	mov   word[Xsize],ax
	mov   ax,word[edi+y_size]
	mov   word[Ysize],ax
	mov   edi,dword[JpegOutput]
	sub   edi,8
	mov   ax,word[Xsize]
	stosd
	mov   ax,word[YsizeImage]     ; Add by C  [Ysize]
	stosd
	pop   eax
	pop   edi
	call  jpeg_display
DeCodeJpegOK:
        mov   esi,iniciomemoria1
        mov   edi,iniciomemoria
        mov   ecx,6
        rep   movsd
	popad
	clc
	ret

DeCodeJpegError:
        mov   esi,iniciomemoria1
        mov   edi,iniciomemoria
        mov   ecx,6
        rep   movsd
	popad
	stc
	ret

put_image:
	pushad
	push  edi
	pop   esi
	push  ax
	xor   eax,eax
	pop   ax
	mov   dword[VesaStartXJpeg],eax
	push  bx
	xor   ebx,ebx
	pop   bx
	mov   dword[VesaStartYJpeg],ebx
	call  GetVesaScreenPointerJpeg
	push  cx
	xor   ecx,ecx
	pop   cx
	push  dx
	xor   edx,edx
	pop   dx
	xor   ebx,ebx
	mov   bx,word[Xsize]
	sub   ebx,ecx
	shl   ebx,2
align 4
abc:
	 push  ecx
	 mov   ecx,edx
align 4
lll:
	 lodsd
	 dec   esi
	 stosd
	 loop lll
	 add   edi,ebx
	 pop   ecx
	 loop  abc
	 popad
	 ret



 ;----------------------------------------------------;
 ; inc_vesa_screen_pointer.                           ;
 ;----------------------------------------------------;
GetVesaScreenPointerJpeg:
	push  edx
	push  eax					      
	push  ebx
	mov   edi,[VesaBufferVar]
	xor   ebx,ebx
       
	mov   ebx,[VesaStartXJpeg]
	shl   ebx,2
@@:
	add   edi, ebx
	mov   ebx,[VesaStartYJpeg]
	xor   eax,eax
	mov   ax,word[Xsize]
	shl   eax,2
@@:
	mul   ebx
	add   edi,eax
	pop   ebx
	pop   eax
	pop   edx
	ret

VesaStartYJpeg dd 0
VesaStartXJpeg dd 0
eaxsave1   dd 0
eaxsave2   dd 0
