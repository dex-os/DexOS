
_null fix 0x1000; 0x1000

 ;----------------------------------------------------;
 ; ReadGIF.                                           ;
 ;----------------------------------------------------;
ReadGIF:
	push  esi edi
	mov   [ColorTableFlag],0
	mov   [GifListAddress],GifList
	mov   [table_ptr],eax
	mov   [cur_info],edi
	mov   eax,GifList
	mov   [GifListAddressNEW],eax
	xor   eax,eax
	mov   [globalColor],eax
	mov   [img_count],eax
	inc   eax
	cmp   dword[esi],'GIF8'
	jne   er		       ; signature
	mov   ecx,[esi+0xa]
	inc   eax
	add   esi,0xd
	mov   edi,esi
	bt    ecx,7
	jnc   nextblock    
	mov   [ColorTableFlag],1      
	mov   [globalColor],esi
	call  Gif_skipmap
nextblock:
	cmp   byte[edi],0x21
	jne   noextblock
	inc   edi
	cmp   byte[edi],0xf9	       ; Graphic Control Ext
	jne   no_gc
	movzx eax,word [edi+3]
	mov   [anim_delay],eax
	mov   al,[edi+2]
	;shr   al,2                    
	;and   eax,7                   
	mov   [anim_disp],eax
	mov   byte[TransparentFag],0
	and   al,1		       
	cmp   al,1		       
	jne   no_transparent		
	mov   byte[TransparentFag],1	  
	mov   al,[edi+5]
	mov   byte[TransparentColor],al
no_transparent:
	mov   eax,[anim_disp]
	shr   al,2		      
	and   eax,7		      
	mov   [anim_disp],eax
	add   edi,7
	jmp   nextblock
no_gc:
	cmp   byte[edi],0xfe	       ; Comment Ext
	jne   no_comm
	inc   edi
block_skip:
	movzx eax,byte[edi]
	lea   edi,[edi+eax+1]
	cmp   byte[edi],0
	jnz   block_skip
	inc   edi
	jmp   nextblock
no_comm:
	cmp   byte[edi],0xff	      ; Application Ext
	jne   nextblock
	add   edi,13
	jmp   block_skip
noextblock:
	cmp   byte[edi],0x2c	      ; image beginning
	jne   er
	inc   [img_count]
	inc   edi
	mov   esi,[cur_info]
	xchg  esi,edi
	mov   bp,word[esi]
	mov   [Xleft],bp

	mov   bp,word[esi+2]
	mov   [Ytop],bp

	mov   bp,word[esi+4]
	mov   [Xgif],bp
	movzx ebx,bp
	mov   [edi],ebx

	mov   bp,word[esi+6]
	mov   [Ygif],bp
	movzx ebx,bp
	mov   [edi+4],ebx

	add   edi,8
	mov   [ImageAddress],edi
	add   esi,8
	push  edi
	movzx ecx,word[esi]
	inc   esi
	bt    ecx,7
	jc    uselocal
	push  [globalColor]
	mov   edi,esi
	jmp   setPal
uselocal:
	call  Gif_skipmap
	push  esi
setPal:
	movzx ecx,byte[edi]
	inc   ecx
	mov   [codesize],ecx
	dec   ecx
	pop   [Palette]
	lea   esi,[edi+1]
	mov   edi,[table_ptr]
	xor   eax,eax
	cld
	lodsb			      ; eax - block_count
	add   eax,esi
	mov   [block_ofs],eax
	mov   [bit_count],8
	mov   eax,1
	shl   eax,cl
	mov   [CC],eax
	inc   eax
	mov   [EOI],eax
	lea   ecx,[eax-1]
	mov   eax, _null shl 16
filltable:
	stosd
	inc   eax
	loop  filltable
	pop   edi
	mov   [img_start],edi
reinit:
	mov   edx,[EOI]
	inc   edx
	push  [codesize]
	pop   [compsize]
	call  Gif_get_sym
	cmp   eax,[CC]
	je    reinit
	call  Gif_output
cycle:
	movzx ebx,ax
	call  Gif_get_sym
	cmp   eax,edx
	jae   notintable
	cmp   eax,[CC]
	je    reinit
	cmp   eax,[EOI]
	je    zend
	call  Gif_output
zadd:
	push  eax
	mov   eax,[table_ptr]
	mov   [eax+edx*4],ebx
	pop   eax
	cmp   edx,0xFFF
	jae   cycle
	inc   edx
	bsr   ebx,edx
	cmp   ebx,[compsize]
	jne   noinc
	inc   [compsize]
noinc:
	jmp   cycle
notintable:
	push  eax
	mov   eax,ebx
	call  Gif_output
	push  ebx
	movzx eax,bx
	call  Gif_output
	pop   ebx eax
	jmp   zadd
er:
	pop   edi
	jmp   ex
zend:
	;mov  eax,[cur_info]          ; skip offset to next frame
	;mov  [eax],edi
	;mov  [cur_info],edi
	mov   ecx,[img_count]
	dec   ecx
	shl   ecx,5
	add   ecx,[GifListAddress]
	mov   ebx,[ImageAddress]
	mov   [ecx],ebx
	mov   ebx,[anim_delay]
	mov   [ecx+4],ebx
	mov   bx,[Xleft]
	mov   word[ecx+8],bx
	mov   bx,[Ytop]
	mov   word[ecx+16],bx
	mov   bx,[Xgif]
	mov   word[ecx+12],bx
	mov   bx,[Ygif]
	mov   word[ecx+14],bx
        mov   ebx,[anim_disp]
	mov   dword[ecx+18],ebx
	mov   bl,byte[TransparentFag]
	mov   bh,byte[TransparentColor]
	mov   byte[ecx+22],bl
	mov   byte[ecx+23],bh
	mov   byte[TransparentFag],0
	mov   [GifListAddressNEW],ecx
	mov   [cur_info],edi
	add   esi,2
	xchg  esi,edi
nxt:
	cmp   byte[edi],0
	jnz   continue
	inc   edi
	jmp   nxt
continue:
	cmp   byte[edi],0x3b
	jne   nextblock
	mov   bl,byte[edi]
	xor   eax,eax
	stosd
	mov   ecx,[img_count]
ex:
	pop   edi esi
	ret

Gif_skipmap:
; in: ecx - image descriptor, esi - pointer to colormap
; out: edi - pointer to area after colormap

	and   ecx,111b
	inc   ecx			
	mov   ebx,1
	shl   ebx,cl
	lea   ebx,[ebx*2+ebx]
	lea   edi,[esi+ebx]
	ret

Gif_get_sym:
	mov   ecx,[compsize]
	push  ecx
	xor   eax,eax
shift:
	ror   byte[esi],1
	rcr   eax,1
	dec   [bit_count]
	jnz   loop1
	inc   esi
	cmp   esi,[block_ofs]
	jb    noblock
	push  eax
	xor   eax,eax
	lodsb
	test  eax,eax
	jnz   nextbl
	mov   eax,[EOI]
	sub   esi,2
	add   esp,8
	jmp   exx
nextbl:
	add   eax,esi
	mov   [block_ofs],eax
	pop   eax
noblock:
	mov   [bit_count],8
loop1:
	loop  shift
	pop   ecx
	rol   eax,cl
exx:
	xor   ecx,ecx
	ret

Gif_output:
	push  esi eax edx
	mov   edx,[table_ptr]
next:
	push  word[edx+eax*4]
	mov   ax,word[edx+eax*4+2]
	inc   ecx
	cmp   ax,_null
	jnz   next
	shl   ebx,16
	mov   bx,[esp]
loop2:
	pop   ax

	lea   esi,[eax+eax*2]
	add   esi,[Palette]

	if    ColorOrder eq Dex4U
	mov   esi,[esi]
	bswap esi
	shr   esi,8
	mov   [edi],esi
	add   edi,3
	else
	movsw
	movsb
	end if

	loop  loop2
	pop   edx eax esi
	ret

ColorTableFlag db 0
				       
globalColor  dd 1
img_count    dd 1
cur_info     dd 1		      
img_start    dd 1
codesize     dd 1
compsize     dd 1
bit_count    dd 1
CC	     dd 1
EOI	     dd 1
Palette      dd 1
block_ofs    dd 1
table_ptr    dd 1

Xgif	     dw 0
Ygif	     dw 0
anim_delay   dd 0
anim_disp    dd 0
TransparentFag	 db 0
TransparentColor db 0
