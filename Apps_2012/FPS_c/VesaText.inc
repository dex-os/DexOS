;=========================================================;
; Vesa_Text                                      11/12/03 ;
;---------------------------------------------------------;
; Vesa small text functions for uses in programs.         ;
;                                                         ;
; (c) Craig Bamford, All rights reserved.                 ;
;=========================================================;
 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Draw Char small32bpp ; draws char from al in vesa. ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      Al Ascii character to write.                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
Draw_Char_small32:
	pushad
	xor   dx,dx
	mov   dl,[Row]	
	cli	     
	cld 
GetData32:	       
	lodsb 
	shl   ax,8		    
	mov   bx,ax	     
	mov   cl,8	    
CheckBit32:		   
	test  bx,8000h	      
	jz    ZeroBit32 
	push  eax      
	mov   eax,[FontColor]  
	stosd
	pop   eax
	jmp   Skip32		
ZeroBit32:
	add   edi,4
Skip32: 		
	shl   bx,1	      
	loop  CheckBit32	 
	add   edi,dword[FontAddOn] ;2528    
	dec   dl		
	jnz   GetData32 
	sti 
	popad	
	ret

 ;----------------------------------------------------;
 ; Converts a string from text to vesa mode8x16 fonts.;
 ;----------------------------------------------------;
Convert_Text_Small16: 
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	shl   eax,4
	lea   esi,[Dex8x16+eax] 
	mov   [tcolor],0ah
	call  get_vesa_screen_pointer
	call  Draw_Char_small32
	call  inc_vesa_screen_pointer
	pop   esi
	pop   eax
	pop   ebx
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts                       ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      es:esi points to asciiz string to write.      ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
print_vesa_string_small16:				     
	pushad
	push  es
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],9
	mov   byte [Row],16
	mov   [FontColor],edx
	xor   eax,eax 
abloop16:
	mov   al,[esi]
	mov   [tchar],al
	cmp   al,0					   ; 0x00 = end of string
	je    gbend16
	cmp   al,0x0D					   ; 0x0d = CR ( = \n )
	jne   nbo_cr16
	call  cage_return
	jmp   ab116
nbo_cr16:
	call  Convert_Text_Small16
ab116:	inc   esi
	jmp   abloop16
gbend16:
	pop   es
	popad
	ret



 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintVesaChar16    ; does what it say's on the box ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      Prints whats in AL,                           ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintVesaChar16:
	pushad
	mov   [FontW],9
	mov   byte [Row],16
	mov   [FontColor],edx
	mov   [tchar],al  
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax   
	call  Convert_Text_Small16
	popad
	ret

 ;----------------------------------------------------;
 ; inc_vesa_screen_pointer.                           ;
 ;----------------------------------------------------;
get_vesa_screen_pointer:
	push  eax					   
	push  ebx
	mov   edi,VesaBuffer
	xor   ebx,ebx
	mov   ebx,[vesa_fontsmall_x]
	shl   ebx,2
	add   edi, ebx
	mov   ebx,[vesa_fontsmall_y]
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2	 
	mul   ebx
	add   edi,eax
	pop   ebx
	pop   eax
	ret

 ;----------------------------------------------------;
 ; inc_vesa_screen_pointer.                           ;
 ;----------------------------------------------------;
inc_vesa_screen_pointer:
	push  ecx
	push  eax
	cld
	mov   eax,[vesa_fontsmall_x] 
	mov   cx,[ModeInfo_XResolution]
	sub   cx,10
	cmp   ax,cx
	jb    no4_incy
	call  cage_return
	jmp   d1end
no4_incy:
	add   eax,[FontW] 
	mov   [vesa_fontsmall_x],eax
d1end:	pop   eax
	pop   ecx
	ret

 ;----------------------------------------------------;
 ; cage_return.                                       ;
 ;----------------------------------------------------;
cage_return:
	push  eax
	mov   [vesa_fontsmall_x], dword 0
	mov   eax,[vesa_fontsmall_y] 
	cmp   eax,467		      
	jb    no_scrl
	mov   [vesa_fontsmall_x],0
	mov   [vesa_fontsmall_y],0
	add   [vesa_fontsmall_y],1
	jmp   cend1
no_scrl:
	add   eax,12	
	mov   [vesa_fontsmall_y],eax	 
cend1:	pop   eax
	ret

 ;----------------------------------------------------;
 ; PrintFPS                                           ;
 ;----------------------------------------------------;
PrintFPS:
	pushad
	mov   al,0x0   ;second
	out   0x70,al
	in    al,0x71
	cmp   al,[lastSecondTick]
	je    PrintFpsExit
	mov   [lastSecondTick],al
	mov   eax,0x00ffffff
	call  FillScreenColorSmall
	call  ConvertXtoDec
	mov   edx,0x00000000
	mov   [vesa_fontsmall_x],8
	mov   [vesa_fontsmall_y],2
	mov   esi,FpsMsg
	call  print_vesa_string_small16
	mov   [FpsCount],0
PrintFpsExit:
	popad
	ret

;====================================================;
; ConvertXtoDec.                                     ;
;====================================================;
ConvertXtoDec:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   esi,str_x
	mov   byte[str_x],' '
	mov   byte[str_x+1],' '
	mov   [str_x+2],' '
	mov   ax,[FpsCount]
	mov   bx,10
loopd3: 
	mov   dx,0
	div   bx
	add   dl,30h
	mov   cl,byte[ds:esi+1]
	mov   byte[ds:esi+2],cl
	mov   cl,byte[ds:esi]
	mov   byte[ds:esi+1],cl
	mov   byte [ds:esi],dl
	cmp   ax,0
	jne   loopd3
	pop   ds
	pop   es
	popad
	ret
 ;----------------------------------------------------;
 ; DATA.                                              ;
 ;----------------------------------------------------;
vesa_fontsmall_x   dd 0
vesa_fontsmall_y   dd 0 			     
textx		   dw 0
texty		   dw 0
Row		   db 0
tcolor		   db 0
FontColor	   dd 0
FontAddOn	   dd 0
tchar		   db 0
color		   dd 0  
red		   dd 0
blue		   dd 0  
bpp		   dd 0
FontCopy	   dw 0
FontW		   dd 0
chel		   db 0
FpsMsg	   db "FPS  "
str_x	   db	'    ',0

