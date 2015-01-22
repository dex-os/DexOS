;====================================================================================;
; RotateImage                                                               12/04/09 ;
;------------------------------------------------------------------------------------;
; By Dex (Craig Bamford.)                                                            ;
;                                                                                    ;
; A simple prog to demo rotating and scaling a bmp image, for  mode 13h.             ;
; c:\fasm Rotate.asm Rotate.dex     <enter>                                          ;
;                                                                                    ;
;====================================================================================;
format binary as 'dex'
use32
	ORG   0x1A00000 				 ; where our program is loaded to
	jmp   start					; jump to the start of program.
	db    'DEX6'					; We check for this, to make shore it a valid DexOS file.
texture = LoadMeHere +128+128+54+ 256*3
msg1:	db 13, " The bmp is bigger than (320x200) 256 color or is not a bmp file,",13
	db " press anykey to exit.",13,0

start:
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
;------------------------------------------------------------
;      Get calltable address.
;------------------------------------------------------------
	mov   edi,Functions
	mov   al,0
	mov   ah,0x0a
	int   50h
	mov   eax,buffer_seg1
	call  DisplayBMP
	jc    quit_me
	xor   eax,eax
	mov   ecx,eax
	mov   ebx,eax
	mov   edx,eax
	; make sine & cosine tables
	finit
	fldz
	xor   edi,edi
	mov   ecx,256
mk_look:
	fld   st
	fld   st
	fsin
	fmul  [mul_const]
	fistp word[sintab+edi]
	fcos
	fmul  [mul_const]
	fistp word[costab+edi]
	fadd  [d_angle]
	add   edi,2
	dec   ecx
	jnz   mk_look
	ffree st
align 4
main_loop:
;------------------------------------------------------------
;       precalculate lookups
;------------------------------------------------------------
	xor   ebx,ebx
	mov   bx,[angle]
	and   ebx,255
	shl   ebx,1
	mov   ax,word [sintab+ebx]
	mov   [_sin],ax
	mov   ax,word [costab+ebx]
	mov   [_cos],ax
	; vertical lookup tables
	xor   edi,edi
	mov   ecx,-64 ;100
pre_v:
	mov   ax,[_sin]
	imul  cx
	shl   edx,16
	mov   edx,eax
	mov   dword[v_sin_lookup+edi],edx
	mov   ax,[_cos]
	imul  ecx
	shl   edx,16
	mov   edx,eax
	mov   dword [v_cos_lookup+edi],edx
	add   edi,4
	inc   ecx
	cmp   ecx,64 ;100
	jne   pre_v
	; horizontal lookup tables
	xor   edi,edi
	mov   ecx,-64 ;160
pre_h:
	mov   ax,[_sin]
	imul  cx
	shl   edx,16
	mov   edx,eax
	mov   dword [h_sin_lookup+edi],edx
	mov   ax,[_cos]
	imul  cx
	shl   edx,16
	mov   edx,eax
	mov   dword[h_cos_lookup+edi],edx
	add   edi,4
	inc   ecx
	cmp   ecx,64 ;160
	jne   pre_h
;------------------------------------------------------------
;       rotate & draw texture
;------------------------------------------------------------
	push  es
	xor   edi,edi
	mov   edi,buffer_seg2

	mov   ecx,128 ;200
	xor   esi,esi
draw_ver:
	push  ecx
	mov   ecx,128 ;320
	xor   ebp,ebp
draw_hor:
	; tex_x = cos(angle)*x - sin(angle)*y
	; tex_x *= scale_const
	mov   eax,dword [h_cos_lookup+ebp]
	sub   eax,dword [v_sin_lookup+esi]
	sar   ax,7	;ccccccc
	mul   [scale_const]
	shrd  ax,dx, 7
	add   ax,64    ;cccccccccccccc
	and   ax,127
	mov   bx,ax
	; tex_y = sin(angle)*x + cos(angle)*y
	; tex_y *= scale_const
	mov   eax,dword [h_sin_lookup+ebp]
	add   eax,dword [v_cos_lookup+esi]
	sar   ax,7
	mul   [scale_const]
	shrd  ax,dx,7
	add   ax,64    ;cccccccccccccc
	and   ax,127
	shl   ax,7
	add   bx,ax
       ; mov   al,byte[es:edi]
	;inc   edi
	mov   al,byte[texture+ebx]
	stosb
	add   ebp,4
	dec   ecx
	jnz   draw_hor
	add   esi,4
	pop   ecx
	dec   ecx
	jnz   draw_ver
	pop   es
;------------------------------------------------------------
;       update angle & scale
;------------------------------------------------------------
	mov   bx,[a_sinpos]
	and   bx,255
	shl   bx,1
	mov   ax,word [sintab+ebx]
	add   [angle],ax
	sar   [angle],1
	inc   [a_sinpos]
;-----------------------------------------------------------
       ; unhighlight this to scale in/out
       ; add     ax,[scale_const]
       ; sar     ax,1
       ; add     ax,31
       ; mov     [scale_const],ax
;------------------------------------------------------------
;;;;;;;;;;;;;;;;;;;;;;;;
	mov   edi,buffer_seg1
	mov   al,0
	mov   ecx,320*200
	rep   stosb
	mov   edi,buffer_seg1
	add   edi,320*36+160-64
	mov   esi,buffer_seg2
	mov   ecx,128
LetsLOOP1:
	 push ecx
	 mov  ecx,128
	 rep  movsb
	 add  edi,320-128
	 pop  ecx
	 loop LetsLOOP1


;;;;;;;;;;;;;;;;;;;;;;;;
	call  retrace
	call  copy_buffer

       ; mov   edi,buffer_seg2
       ; mov   al,0
       ; mov   ecx,128*128
       ; rep   stosb
	call  [WaitForKeyPress]
	in    al,60h
	dec   al
	jnz   main_loop
quit_me:
	mov   ax,0x0003
	call  [RealModeInt10h]
	call  [Clstext]
	ret


;;;;;;;;;;;;;;;;;;;;;;;;;;;;
RotateXY:

   ; push    ds
   ; mov     ax,cs                   ;the basic formula for rotations:
   ; mov     ds,ax                   ; X := cos(angle) * x - sin(Xan) * y
				    ; Y := sin(angle) * x + cos(Xan) * y
    mov     esi,[Angle]
    add     esi,esi		      ; si = angle*2
    mov     ax,word[Cosine+esi] 	 ; ax = cos(angle)
    imul    bx			    ; ax = cos(angle) * x
    shl     edx,16		    ; put dx in high edx
    mov     dx,ax		    ; save all 32 bits
    mov     edi,edx		    ; store for later use

    mov     ax,word[Sine+esi]		 ; ax = sin(angle)
    imul    cx			    ; ax = sin(angle) * y
    shl     edx,16
    mov     dx,ax
    sub     edi,edx		    ; edi = edi-eax=cos(angle)*x-sin(angle)*y
    sar     edi,8		    ; remove the "256-factor"
    mov     ebp,edi		    ; ebp = x-coordinate

    mov     ax,[Sine+esi]	     ; ax = sin(angle x)
    imul    bx			    ; ax = sin(angle x) * x
    shl     edx,16
    mov     dx,ax
    mov     edi,edx

    mov     ax,[Cosine+esi]	     ; ax = cos(angle x)
    imul    cx			    ; ax = cos(angle x) * y
    shl     edx,16
    mov     dx,ax
    add     edi,edx		    ; di = di-ax = sin(vx)*y + cos(vx)*z
    sar     edi,8		    ; remove the (co)sin "256-factor"

    mov     ebx,ebp		      ; update X
    mov     ecx,edi		      ; update Y
    shl     ecx,16
    shr     ecx,16
    shl     ebx,16
    shr     ebx,16
    ret
;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;------------------------------------------------------------
;  Data
;------------------------------------------------------------
Angle  dd 0
align 4
include "Graphic.inc"

Sine	dw   0,6,13,19,25,31,38,44,50,56
	dw   62,69,75,81,87,92,98,104,110,116  
	dw   121,127,132,137,143,148,153,158,163,168  
	dw   172,177,182,186,190,194,198,202,206,210  
	dw   213,217,220,223,226,229,232,235,237,239  
	dw   241,243,245,247,249,250,251,252,253,254  
	dw   255,255,256,256	    ;64 vlaues

Cosine	dw   256,256,256,255,255,254
	dw   253,252,251,249,248,246,244,242,240,238  
	dw   236,233,231,228,225,222,218,215,212,208  
	dw   204,200,196,192,188,184,179,175,170,165  
	dw   160,156,150,145,140,135,129,124,118,113  
	dw   107,101,95,90,84,78,72,65,59,53  
	dw   47,41,35,28,22,16,9,3,0 ,-4 ,-10  
	dw  -16 ,-23 ,-29 ,-35 ,-41 ,-48 ,-54 ,-60 ,-66 ,-72  
	dw  -78 ,-84 ,-90 ,-96 ,-102 ,-107 ,-113 ,-119 ,-124 ,-130  
	dw  -135 ,-141 ,-146 ,-151 ,-156 ,-161 ,-166 ,-171 ,-175 ,-180	
	dw  -184 ,-189 ,-193 ,-197 ,-201 ,-205 ,-209 ,-212 ,-216 ,-219	
	dw  -222 ,-225 ,-228 ,-231 ,-234 ,-236 ,-239 ,-241 ,-243 ,-245	
	dw  -247 ,-248 ,-250 ,-251 ,-252 ,-253 ,-254 ,-255 ,-256 ,-256	
	dw  -256 ,-256 ,-256 ,-256 ,-256 ,-255 ,-255 ,-254 ,-253 ,-252	
	dw  -251 ,-249 ,-248 ,-246 ,-244 ,-242 ,-240 ,-237 ,-235 ,-232	
	dw  -230 ,-227 ,-224 ,-221 ,-217 ,-214 ,-210 ,-207 ,-203 ,-199	
	dw  -195 ,-191 ,-186 ,-182 ,-178 ,-173 ,-168 ,-163 ,-159 ,-154	
	dw  -148 ,-143 ,-138 ,-133 ,-127 ,-122 ,-116 ,-110 ,-105 ,-99  
	dw  -93 ,-87 ,-81 ,-75 ,-69 ,-63 ,-57 ,-51 ,-44 ,-38  
	dw  -32 ,-26 ,-19 ,-13 ,-7
	dw   0,6,13,19,25,31,38,44,50,56  
	dw   62,69,75,81,87,92,98,104,110,116  
	dw   121,127,132,137,143,148,153,158,163,168  
	dw   172,177,182,186,190,194,198,202,206,210  
	dw   213,217,220,223,226,229,232,235,237,239  
	dw   241,243,245,247,249,250,251,252,253,254  
	dw   255,255,256,256

d_angle      dd 0.024543693   ;0.0122718463 ;0.024543693                  ; pi/128
mul_const    dd 128.0

angle	     dw 0
a_sinpos     dw 0

scale_const  dw 128

sintab	     dw 256 dup(?)
costab	     dw 256 dup(?)

v_sin_lookup dd 200 dup(?)
v_cos_lookup dd 200 dup(?)

h_sin_lookup dd 320 dup(?)
h_cos_lookup dd 320 dup(?)

_sin	     dw ?
_cos	     dw ?

_pre1	     dd ?
_pre2	     dd ?

align 4
LoadMeHere:
file  'image3.bmp'	   ; include your bmp like this (put the name of your bmp here)
			   ; no bigger than 320x200 8bpp in this case
packer:      rb  640*400   ; This not realy needed, but i like to have things spaced out
align 4
include 'Dex.inc'
align 4
ImageBuffer  rb 320*200
align 4
buffer_seg1  rb 320*200
align 4
buffer_seg2  rb 320*200