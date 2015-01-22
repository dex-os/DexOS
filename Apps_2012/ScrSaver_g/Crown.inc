 ;----------------------------------------------------;
 ; filling_alfbet. fill table alfbet by casual numbers;
 ;----------------------------------------------------;
filling_alfbet: 		    ; Initialize RND
	pushad
	call  [TimerSecondCount]   
	; eax - fist random number
	mov   ecx,n_points
	mov   edi,alfbet
	mov   ebx,8088405h
ck2:
	stosd
	; Compute next random number
	; New := 8088405H * Old + 1
	mul   ebx
	inc   eax
	loop ck2
	popad 
	ret

 ;----------------------------------------------------;
 ; BuffToScreen.  ;Puts whats in the buffer to screen ;
 ;----------------------------------------------------;
BuffToScreen:
	cmp   [ModeInfo_BitsPerPixel],24
	jne   Try32
	call  BuffToScreen24
	jmp   wehavedone24
Try32:
	cmp   [ModeInfo_BitsPerPixel],32
	jne   wehavedone24
	call  BuffToScreen32 
wehavedone24:
	ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               32bpp ;
 ;----------------------------------------------------;
BuffToScreen24:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 mov   edi,[VesaScreen] ;[ModeInfo_PhysBasePtr]
	 mov   esi,scr
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ebx,eax
	 mov   bx,[ModeInfo_XResolution]
	 sub   ebx,480
	 mov   [ImageXaddOn24],ebx
	 shl   ebx,2
	 sub  ebx,[ImageXaddOn24]
	 mov  [ImageXaddOn],ebx
	 mov   ecx,480

testloop24:
	 push  ecx
	 mov   ecx,480
	 cld
@@:
	 lodsd
	 dec   esi
	 stosd
	 dec   edi
	 loop @b
	 add   edi,[ImageXaddOn]
	 pop   ecx
	 loop  testloop24
	 pop   es
	 popad
	 ret

 ;----------------------------------------------------;
 ; BuffToScreen24                               24bpp ;
 ;----------------------------------------------------;
BuffToScreen32:
	 pushad
	 push  es
	 mov   ax,8h
	 mov   es,ax
	 mov   edi,[VesaScreen] ;[ModeInfo_PhysBasePtr]
	 mov   esi,scr
	 xor   eax,eax
	 mov   ecx,eax
	 mov   ebx,eax
	 mov   bx,[ModeInfo_XResolution]
	 sub   ebx,480
	 shl   ebx,2
	 mov   ecx,480
testloop32:
	 push  ecx
	 mov   ecx,480
	 cld
@@:	 lodsd

	 stosd
	 dec   esi
	 loop @b
	 add   edi,ebx
	 pop   ecx
	 loop  testloop32
	 pop   es
	 popad
	 ret

 ;----------------------------------------------------;
 ; calcframe.                                         ;
 ;----------------------------------------------------;
calcframe:
	cld
	mov   edi,scr
	mov   ecx,480*480*4
	xor   eax,eax
	rep   stosd	; CLS
	mov   ebx,[frame]
	not   bh
	test  bh,03h
	not   bh
	jnz   lb1
	; ebx=xxxx xxxx  xxxx xxxx  xxxx xx11  xxxx xxxxb
	mov   byte [p],bl
lb1:
	test  bx,03ffh
	jnz   lb2
	; ebx=xxxx xxxx  xxxx xxxx  xxxx xx00  0000 0000b
	mov   [p],0
	mov   eax,[pp1adr]
	mov   [pp0adr],eax
	inc   [pp1]
	cmp   [pp1],num_tabl_calls
	jnz   lb3
	mov   [pp1],0
lb3:
	movzx eax,[pp1]
	mov   eax,[tabl_calls+eax*4]
	mov   [pp1adr],eax
lb2:
	fild  [frame]	 ; st0=STime
	fmul  [speed1]	 ; st0=STime*Speed
	fst   [bt_r]	 ; al_rSTime*Speed
	fadd  st0,st0
	fstp  [al_r]	 ; al_rSTime*Speed*2
	mov   [Fl],0
	mov   ecx,[mFl]
	mov   esi,alfbet
ckl1:
       call  [pp0adr]
       cmp   [p],0
jz  lb4
       fstp  [x1]
       fstp  [y1]
       fstp  [z1]
       call  [pp1adr]
       call  mix
lb4:
       call  turn
       add   esi,4
       inc   [Fl]
       loop  ckl1
       inc   ebx
       mov   [frame],ebx
       ret

 ;----------------------------------------------------;
 ; turn coordinate system.                            ;
 ;----------------------------------------------------;
turn:
	; around Y
	;  x= x*cos(a)-z*sin(a)
	;  y= y
	;  z= x*sin(a)+z*cos(a)
	fld  st2      ; st0=z  st1=x  st2=y  st3=z
	fld  st1      ; st0=x  st1=z  st2=x  st3=y  st4=z
	fld  [al_r]   ; st0=a  st1=x  st2=z  st3=x  st4=y  st5=z
	fsincos       ; st0=cos(a)  st1=sin(a)  st2=x  st3=z  st4=x  st5=y  st6=z
	fmul  st4,st0
	fmulp st6,st0 ; st0=sin(a)  st1=x  st2=z  st3=x*cos(a)  st4=y  st5=z*cos(a)
	fmul  st2,st0
	fmulp st1,st0 ; st0=x*sin(a)  st1=z*sin(a)  st2=x*cos(a) st3=y st4=z*c
	faddp st4,st0
	fsubp st1,st0

	; around X
	;  x=x
	;  y= y*cos(b)+z*sin(b)
	;  z=-y*sin(b)+z*cos(b)
	fld  st2      ; st0=z  st1=x  st2=y  st3=z
	fld  st2      ; st0=y  st1=z  st2=x  st3=y  st4=z
	fld  [bt_r]   ; st0=b  st1=y  st2=z  st3=x  st4=y  st5=z
	fsincos       ; st0=cos(b)  st1=sin(b)  st2=y  st3=z  st4=x  st5=y  st6=z
	fmul  st5,st0
	fmulp st6,st0 ; st0=sin(b)  st1=y  st2=z  st3=x  st4=y*cos(b)  st5=z*cos(b)
	fmul  st2,st0
	fmulp st1,st0 ; st0=y*sin(b) st1=z*sin(b) st2=x st3=y*cos(b) st4=z*cos(b)
	fsubp st4,st0 ; st0=z*sin(b)  st1=x  st2=y*cos(b)  st3=z*cos(b)-y*sin(b)
	faddp st2,st0

	; st0=x  st1=y  st2=z
	fistp  [x1]
	fistp  [y1]
	fmul   [Zdepth]   ; st0=z*Zdepth
	fiadd  [Zcolor]   ; st0=z*Zdepth+Zcolor
	fistp  [z_w]	  ; st0z*Zdepth+Zcolor
	push   edx

	mov    eax,[x1]
	add    eax,[mid]
	mul    [const480]
	add    eax,[y1]
	add    eax,[mid]
	mul    [const3]

	mov   dl,byte [z_w]	 ; al=ZZ
	mov   [scr+0+eax],dl
	mov   [scr+1+eax],dl
	mov   [scr+2+eax],dl
	pop   edx
	ret

 ;----------------------------------------------------;
 ; mix.                                               ;
 ;----------------------------------------------------;
mix:
	fild  [p]	; st0=p
	fmul  [mp]	; st0=p=p*mp
	fld   st0	; st0=p  st1=p
	fmul  st4,st0
	fmul  st3,st0
	fmulp st2,st0	; st0=p    st1=x*p  st2=y*p  st3=z*p
	fld1
	fsubrp st1,st0	; st0=1-p  st1=x*p  st2=y*p  st3=z*p
	fld    [z1]	; st0=z1   st1=1-p  st2=x*p  st3=y*p  st4=z*p
	fmul  st0,st1
	faddp st4,st0
	fld    [y1]	; st0=y1   st1=1-p  st2=x*p  st3=y*p  st4=
	fmul  st0,st1
	faddp st3,st0
	fld    [x1]	; st0=x1   st1=1-p  st2=x*p  st3=y*p+y1*(1-p)  st4=
	fmulp st1,st0	; st0=x1*(1-p)      st1=x*p  st2=y*p+y1*(1-p)  st3=
	faddp st1,st0	; st0=x=x*p+x1*(1-p)  st1=y=y*p+y1*(1-p)  st2=z
	ret

 ;----------------------------------------------------;
 ; GetWinBufferPointer.                               ;
 ;----------------------------------------------------;
GetWinBufferPointer:					 
	push  eax					      
	push  ebx
	mov   eax,[WinStartX]
	mov   [WinBuffX],eax
	mov   ebx,[WinStartY]
	mov   [WinBuffY],ebx
	mov   edi,[ModeInfo_PhysBasePtr]				 
	xor   ebx,ebx
	mov   ebx,[WinBuffX]
	shl   ebx,2
	cmp   [ModeInfo_BitsPerPixel],24   
	jne   @F
	sub   ebx,[WinBuffX] 
@@:	  
	add   edi, ebx
	mov   ebx,[WinBuffY]
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution] 
	shl   eax,2 
	     
	cmp   [ModeInfo_BitsPerPixel],24    
	jne   @f
	sub   ax,[ModeInfo_XResolution] 
@@:
	mul   ebx
	add   edi,eax
	mov   dword[VesaScreen],edi
	pop   ebx
	pop   eax
	ret
VesaScreen    dd  0
ImageXaddOn24 dd  0
ImageXaddOn   dd  0
WinBuffY      dd  0
WinBuffX      dd  0
