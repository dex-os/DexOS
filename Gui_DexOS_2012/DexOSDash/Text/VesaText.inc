;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.05                                                       ;;
;; Author           :   Craig Bamford (a.k.a Dex)                                   ;;
;; Website          :   www.dex-os.com                                              ;;
;; Forum            :   http://dex.7.forumer.com                                    ;;
;; Date             :   Mar 29, 2011                                                ;;
;; Filename         :   Gui.inc                                                    ;;
;; Copy Right Owner :   Craig Bamford                                               ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Team DexOS       :   0x4e71, bubach, Dex, hidnplayr, jas2o, steve, Cloud         ;;
;;                  :   smiddy, viki.                                               ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Disclaimer       :                                                               ;;
;; This software is provided "AS IS" without warranty of any kind, either           ;;
;; expressed or implied, including, but not limited to, the implied                 ;;
;; warranties of merchantability and fitness for a particular purpose. The          ;;
;; entire risk as to the quality and performance of this software is with           ;;
;; you.                                                                             ;;
;; In no event will the author's, distributor or any other party be liable to       ;;
;; you for damages, including any general, special, incidental or                   ;;
;; consequential damages arising out of the use, misuse or inability to use         ;;
;; this software (including but not limited to loss of data or losses               ;;
;; sustained by you or third parties or a failure of this software to operate       ;;
;; with any other software), even if such party has been advised of the             ;;
;; possibility of such damages.                                                     ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Thanks to Alexei Frounze, Tomasz Grysztar, Jim Hall, Pat Villani, Pype.Clicker.  ;;
;;                                                                                  ;;
;; Also a thanks to  the many forums i am a member of, i would like to thank anyone ;;
;; who  has helped me, by answering my ? (too many too list).                       ;;
;;                                                                                  ;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;=========================================================;
; Small_vesa_Text                                11/12/03 ;
;---------------------------------------------------------;
; Vesa small text functions for uses in programs.         ;
;                                                         ;
; Dex4u V0.01                                             ;
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
	xor   edx,edx
	xor   ebx,ebx
	xor   eax,eax
	xor   ecx,ecx
	mov   dl,[Row]	
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
	popad
	ret

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

Draw_Char_small32_BGColor:
	pushad
	xor   edx,edx
	xor   ebx,ebx
	xor   eax,eax
	xor   ecx,ecx
	mov   dl,[Row]	
	cld
GetData32_BGColor:	       
	lodsb 
	shl   ax,8		    
	mov   bx,ax	     
	mov   cl,8	    
CheckBit32_BGColor:		   
	test  bx,8000h	      
	jz    ZeroBit32_BGColor 
	push  eax      
	mov   eax,[FontColor]  
	stosd
	pop   eax
	jmp   Skip32_BGColor		
ZeroBit32_BGColor:
	push  eax      
	mov   eax,[FontColorBG]  
	stosd
	pop   eax
Skip32_BGColor: 		
	shl   bx,1	      
	loop  CheckBit32_BGColor	 
	add   edi,dword[FontAddOn] 
	dec   dl
	jnz   GetData32_BGColor
	popad
	ret

 ;----------------------------------------------------;
 ; Converts a string from text to vesa mode8x8 fonts. ;
 ;----------------------------------------------------;
Convert_Text_Small8: 
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	lea   esi,[Dex8x8+eax*9]  
	mov   [tcolor],0ah
	call  get_vesa_screen_pointer
	call  Draw_Char_small32
	call  inc_vesa_screen_pointer
	pop   esi
	pop   eax
	pop   ebx
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
 ; Prints vesa string 8x8fonts                        ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      es:esi points to asciiz string to write.      ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
print_vesa_string_small8:				    
	pushad
	push  es
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax

	mov   [FontW],6
	mov   byte [Row],9
	;mov   [FontColor],edx
	xor   eax,eax 
abloop8:
	mov   al,[esi]
	mov   [tchar],al
	cmp   al,0					   ; 0x00 = end of string
	je    gbend8
	cmp   al,0x0D					   ; 0x0d = CR ( = \n )
	jne   nbo_cr8
	call  cage_return
	jmp   ab18
nbo_cr8:
	call  Convert_Text_Small8
ab18:	inc   esi
	jmp   abloop8
gbend8:
	pop   es
	popad
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
	;mov   [FontColor],edx
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
 ; PrintVesaChar8     ; does what it say's on the box ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      Prints whats in AL, 8x8 fonts.                ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;

PrintVesaChar8: 				  
	pushad
	mov   [FontW],6
	mov   byte [Row],9
	mov   [FontColor],edx
	mov   [tchar],al 
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax  
	call  Convert_Text_Small8
	popad
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintVesaChar8     ; does what it say's on the box ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      Prints whats in AL, 8x8 fonts.                ;
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
 ; render's graphic screen.                           ;
 ;----------------------------------------------------;

render_screen:
	mov   edi,[ModeInfo_PhysBasePtr]     
	mov   ecx,640*480
	mov   eax, [color]
	cld
	cli
bbe:
	stosd
	cmp   [ModeInfo_BitsPerPixel],24
	jne   @f
	sub   edi,1
@@:
	loop  bbe      
	sti
	ret

 ;----------------------------------------------------;
 ; inc_vesa_screen_pointer.                           ;
 ;----------------------------------------------------;
;get_vesa_screen_pointer:
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
get_vesa_screen_pointer:
	push  eax					   
	push  ebx
	xor   eax,eax
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	mov   [BytesPerScanLine],ebx
	mov   eax,[vesa_fontsmall_x]
	mov   ebx,[vesa_fontsmall_y] 
	imul  ebx,[BytesPerScanLine]	; ebx = y * y multiplier
	lea   edi,[ebx+eax*4]		; edi = x*4+(y*y multiplier)
	add   edi,dword[SetVesaBuffer]	;VesaBuffer             ; ebx = where pixel is in memory
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
	push  ebx
	mov   [vesa_fontsmall_x], dword 0
	mov   eax,[vesa_fontsmall_y]
	xor   ebx,ebx
	mov   bx,[ModeInfo_YResolution]
	sub   bx,13
	cmp   eax,ebx ;467
	jb    no_scrl
	mov   [vesa_fontsmall_x],0
	mov   [vesa_fontsmall_y],0
	add   [vesa_fontsmall_y],1
	jmp   cend1
no_scrl:
	add   eax,12	
	mov   [vesa_fontsmall_y],eax

cend1:	pop   ebx
	pop   eax
	ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                                                                    ;
;      ****** HERE ARE A NEW SET OF BITMAP FONT PRINT FUNCTIONS, FOR THE MENUET TO DEXOS FUNCTION WRAP. ********     ;
;                                                                                                                    ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; BitMapFontsSetXY                                   ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
BitMapFontsSetXY:
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintBitMapFonts                                   ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;    ES:ESI points to asciiz string (zero ended).    ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ;       EDX color xxRRGGBB                           ;
 ;       ECX 1 = 8x8fonts                             ;
 ;           2 = 8x16fonts                            ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintBitMapFonts:
	pushad
	push  es
	mov   [FontColor],edx
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	mov   [FontColor],edx
	cmp   ecx,1
	jne   Try8x16fonts
	call  print_vesa_string_small8x8M2D
	jc    PrintBitMapFontsExitError
	jmp   PrintBitMapFontsExit
Try8x16fonts:
	cmp   ecx,2
	jne   PrintBitMapFontsExitError
	call  print_vesa_string_small8x16M2D
	jc    PrintBitMapFontsExitError
PrintBitMapFontsExit:
	pop   es
	popad
	clc
	ret

PrintBitMapFontsExitError:
	pop   es
	popad
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintBitMapFonts_BGColor                           ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;    ES:ESI points to asciiz string (zero ended).    ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ;       EDX color xxRRGGBB                           ;
 ;       EDI background color xxRRGGBB                ;
 ;       ECX 1 = 8x8fonts                             ;
 ;           2 = 8x16fonts                            ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintBitMapFonts_BGColor:
	pushad
	push  es
	mov   [FontColor],edx
	mov   dword[FontColorBG],edi
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	mov   [FontColor],edx
	cmp   ecx,1
	jne   Try8x16fonts_BGColor
	call  print_vesa_string_small8x8M2D_BGColor
	jc    PrintBitMapFontsExitError
	jmp   PrintBitMapFontsExit
Try8x16fonts_BGColor:
	cmp   ecx,2
	jne   PrintBitMapFontsExitError_BGColor
	call  print_vesa_string_small8x16M2D_BGColor
	jc    PrintBitMapFontsExitError_BGColor
PrintBitMapFontsExit_BGColor:
	pop   es
	popad
	clc
	ret

PrintBitMapFontsExitError_BGColor:
	pop   es
	popad
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintBitMapFontsLen                                ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;    ES:ESI points to asciiz string (zero ended).    ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ;       EDX color xxRRGGBB                           ;
 ;       ECX 1 = 8x8fonts                             ;
 ;           2 = 8x16fonts                            ;
 ;       EDI   = String len                           ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintBitMapFontsLen:
	pushad
	push  es
	mov   [FontColor],edx
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	mov   [FontColor],edx
	cmp   ecx,1
	jne   Try8x16fontsLen
	call  print_vesa_string_small8x8M2DLen
	jc    PrintBitMapFontsExitErrorLen
	jmp   PrintBitMapFontsExitLen
Try8x16fontsLen:
	cmp   ecx,2
	jne   PrintBitMapFontsExitErrorLen
	call  print_vesa_string_small8x16M2DLen
	jc    PrintBitMapFontsExitErrorLen
PrintBitMapFontsExitLen:
	pop   es
	popad
	clc
	ret

PrintBitMapFontsExitErrorLen:
	pop   es
	popad
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintBitMapFontsLen                                ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;    ES:ESI points to asciiz string (zero ended).    ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ;       EDX color xxRRGGBB                           ;
 ;       ECX 1 = 8x8fonts                             ;
 ;           2 = 8x16fonts                            ;
 ;       EDI   = String len                           ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintBitMapFontsLen_BGColor:
	pushad
	push  es
	mov   [FontColor],edx
        mov   dword[FontColorBG],ebx
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	mov   [FontColor],edx
	cmp   ecx,1
	jne   Try8x16fontsLen_BGColor
	call  print_vesa_string_small8x8M2DLen_BGColor
	jc    PrintBitMapFontsExitErrorLen_BGColor
	jmp   PrintBitMapFontsExitLen_BGColor
Try8x16fontsLen_BGColor:
	cmp   ecx,2
	jne   PrintBitMapFontsExitErrorLen_BGColor
	call  print_vesa_string_small8x16M2DLen_BGColor
	jc    PrintBitMapFontsExitErrorLen_BGColor
PrintBitMapFontsExitLen_BGColor:
	pop   es
	popad
	clc
	ret

PrintBitMapFontsExitErrorLen_BGColor:
	pop   es
	popad
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x8fonts M2D ver                ;
 ;----------------------------------------------------;
print_vesa_string_small8x8M2D:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax			   ; width of buffer to add on
	mov   [FontW],6 				   ; width of font
	mov   byte [Row],9				   ; hight of font
	xor   eax,eax 
print_vesa_string_small8x8M2DLoop:
	mov   al,[esi]
	mov   [tchar],al				   ; save font
	cmp   al,0					   ; 0   = end of string
	je    print_vesa_string_small8x8M2DExit
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D1
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D1
	call  Convert_Text_Small8M2D
	jc    print_vesa_string_small8x8M2DExitError
Number2Small_BigM2D1:
	inc   esi
	jmp   print_vesa_string_small8x8M2DLoop
print_vesa_string_small8x8M2DExit:
	clc
	ret
print_vesa_string_small8x8M2DExitError:
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x8fonts M2D ver                ;
 ;----------------------------------------------------;
print_vesa_string_small8x8M2D_BGColor:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax			   ; width of buffer to add on
	mov   [FontW],6 				   ; width of font
	mov   byte [Row],9				   ; hight of font
	xor   eax,eax 
print_vesa_string_small8x8M2DLoop_BGColor:
	mov   al,[esi]
	mov   [tchar],al				   ; save font
	cmp   al,0					   ; 0   = end of string
	je    print_vesa_string_small8x8M2DExit_BGColor
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D1_BGColor
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D1_BGColor
	call  Convert_Text_Small8M2D_BGColor
	jc    print_vesa_string_small8x8M2DExitError_BGColor
Number2Small_BigM2D1_BGColor:
	inc   esi
	jmp   print_vesa_string_small8x8M2DLoop_BGColor
print_vesa_string_small8x8M2DExit_BGColor:
	clc
	ret
print_vesa_string_small8x8M2DExitError_BGColor:
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts M2D ver               ;
 ;----------------------------------------------------;
print_vesa_string_small8x16M2D:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],9
	mov   byte [Row],16
	xor   eax,eax 
print_vesa_string_small8x16M2DLoop:
	mov   al,[esi]
	mov   [tchar],al
	cmp   al,0					   ; 0x00 = end of string
	je    print_vesa_string_small8x16M2DExit
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D2
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D2
	call  Convert_Text_Small16M2D
	jc    print_vesa_string_small8x16M2DExitError
Number2Small_BigM2D2:
	inc   esi
	jmp   print_vesa_string_small8x16M2DLoop
print_vesa_string_small8x16M2DExit:
	clc
	ret
print_vesa_string_small8x16M2DExitError:
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts M2D ver_BGColor       ;
 ;----------------------------------------------------;
print_vesa_string_small8x16M2D_BGColor:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],8 ;9
	mov   byte [Row],16
	xor   eax,eax 
print_vesa_string_small8x16M2DLoop_BGColor:
	mov   al,[esi]
	mov   [tchar],al
	cmp   al,0					   ; 0x00 = end of string
	je    print_vesa_string_small8x16M2DExit_BGColor
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D2_BGColor
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D2_BGColor
	call  Convert_Text_Small16M2D_BGColor
	jc    print_vesa_string_small8x16M2DExitError_BGColor
Number2Small_BigM2D2_BGColor:
	inc   esi
	jmp   print_vesa_string_small8x16M2DLoop_BGColor
print_vesa_string_small8x16M2DExit_BGColor:
	clc
	ret
print_vesa_string_small8x16M2DExitError_BGColor:
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x8fonts M2D ver  Len           ;
 ;----------------------------------------------------;
print_vesa_string_small8x8M2DLen:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax			   ; width of buffer to add on
	mov   [FontW],6 				   ; width of font
	mov   byte [Row],9				   ; hight of font
	xor   eax,eax 
	mov   ecx,edi					   ; string count
print_vesa_string_small8x8M2DLoopLen:
	mov   al,[esi]
	mov   [tchar],al				   ; save font
	;cmp   al,0                                        ; 0   = end of string
	;je    print_vesa_string_small8x8M2DExitLen
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D1Len
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D1Len
	call  Convert_Text_Small8M2D
	jc    print_vesa_string_small8x8M2DExitErrorLen
Number2Small_BigM2D1Len:
	inc   esi
	loop   print_vesa_string_small8x8M2DLoopLen
print_vesa_string_small8x8M2DExitLen:
	clc
	ret
print_vesa_string_small8x8M2DExitErrorLen:
	stc
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts M2D ver  Len          ;
 ;----------------------------------------------------;
print_vesa_string_small8x16M2DLen:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],9
	mov   byte [Row],16
	xor   eax,eax 
	mov   ecx,edi					   ; string count
print_vesa_string_small8x16M2DLoopLen:
	mov   al,[esi]
	mov   [tchar],al
	;cmp   al,0                                        ; 0x00 = end of string
	;je    print_vesa_string_small8x16M2DExitLen
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D2Len
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D2Len
	call  Convert_Text_Small16M2D
	jc    print_vesa_string_small8x16M2DExitErrorLen
Number2Small_BigM2D2Len:
	inc   esi
	loop   print_vesa_string_small8x16M2DLoopLen
print_vesa_string_small8x16M2DExitLen:
	clc
	ret
print_vesa_string_small8x16M2DExitErrorLen:
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x8fonts M2D ver  Len           ;
 ;----------------------------------------------------;
print_vesa_string_small8x8M2DLen_BGColor:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax			   ; width of buffer to add on
	mov   [FontW],6 				   ; width of font
	mov   byte [Row],9				   ; hight of font
	xor   eax,eax 
	mov   ecx,edi					   ; string count
print_vesa_string_small8x8M2DLoopLen_BGColor:
	mov   al,[esi]
	mov   [tchar],al				   ; save font
	;cmp   al,0                                        ; 0   = end of string
	;je    print_vesa_string_small8x8M2DExitLen_BGColor
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D1Len_BGColor
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D1Len_BGColor
	call  Convert_Text_Small8M2D_BGColor
	jc    print_vesa_string_small8x8M2DExitErrorLen_BGColor
Number2Small_BigM2D1Len_BGColor:
	inc   esi
	loop   print_vesa_string_small8x8M2DLoopLen_BGColor
print_vesa_string_small8x8M2DExitLen_BGColor:
	clc
	ret
print_vesa_string_small8x8M2DExitErrorLen_BGColor:
	stc
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts M2D ver  Len          ;
 ;----------------------------------------------------;
print_vesa_string_small8x16M2DLen_BGColor:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],8
	mov   byte [Row],16
	xor   eax,eax 
	mov   ecx,edi					   ; string count
print_vesa_string_small8x16M2DLoopLen_BGColor:
	mov   al,[esi]
	mov   [tchar],al
	;cmp   al,0                                        ; 0x00 = end of string
	;je    print_vesa_string_small8x16M2DExitLen_BGColor
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D2Len_BGColor
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D2Len_BGColor
	call  Convert_Text_Small16M2D_BGColor
	jc    print_vesa_string_small8x16M2DExitErrorLen_BGColor
Number2Small_BigM2D2Len_BGColor:
	inc   esi
	loop   print_vesa_string_small8x16M2DLoopLen_BGColor
print_vesa_string_small8x16M2DExitLen_BGColor:
	clc
	ret
print_vesa_string_small8x16M2DExitErrorLen_BGColor:
	stc
	ret

 ;------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x8 fonts M2D ver. ;
 ;------------------------------------------------------------;
Convert_Text_Small8M2D:
	push  ecx 
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	lea   esi,[Dex8x8+eax*9]  
	call  get_vesa_screen_pointerM2D
	call  Draw_Char_small32
	call  inc_vesa_screen_pointerM2D
	jc    Convert_Text_Small8M2DExitError
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	clc
	ret

Convert_Text_Small8M2DExitError:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x16 fonts M2D ver.;
 ;------------------------------------------------------------;
Convert_Text_Small16M2D:
	push  ecx
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	shl   eax,4
	lea   esi,[Dex8x16+eax] 
	call  get_vesa_screen_pointerM2D
	call  Draw_Char_small32
	call  inc_vesa_screen_pointerM2D
	jc    Convert_Text_Small16M2DExitError
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	ret

Convert_Text_Small16M2DExitError:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x8 fonts M2D ver. ;
 ; _BGColor 
 ;------------------------------------------------------------;
Convert_Text_Small8M2D_BGColor:
	push  ecx 
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	lea   esi,[Dex8x8+eax*9]  
	call  get_vesa_screen_pointerM2D
	call  Draw_Char_small32_BGColor
	call  inc_vesa_screen_pointerM2D
	jc    Convert_Text_Small8M2DExitError_BGColor
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	clc
	ret

Convert_Text_Small8M2DExitError_BGColor:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x16 fonts M2D ver.;
 ; _BGColor 
 ;------------------------------------------------------------;
Convert_Text_Small16M2D_BGColor:
	push  ecx
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	shl   eax,4
	lea   esi,[Dex8x16+eax] 
	call  get_vesa_screen_pointerM2D
	call  Draw_Char_small32_BGColor
	call  inc_vesa_screen_pointerM2D
	jc    Convert_Text_Small16M2DExitError_BGColor
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	ret

Convert_Text_Small16M2DExitError_BGColor:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;-----------------------------------------------------------;
 ; gets vesa screen pointer M2D ver.                         ;
 ;-----------------------------------------------------------;
get_vesa_screen_pointerM2D:
	push  eax					   
	push  ebx
	xor   eax,eax
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	mov   [BytesPerScanLine],ebx
	mov   eax,dword[vesa_fontsmall_x_M2D]
	mov   ebx,dword[vesa_fontsmall_y_M2D]
	imul  ebx,[BytesPerScanLine]			      ; ebx = y * y multiplier
	lea   edi,[ebx+eax*4]				      ; edi = x*4+(y*y multiplier)
	add   edi,dword[SetVesaBuffer]			      ; ebx = where pixel is in memory
	pop   ebx
	pop   eax
	ret

 ;----------------------------------------------------;
 ; inc_vesa_screen_pointer.                           ;
 ;----------------------------------------------------;
inc_vesa_screen_pointerM2D:
	push  ecx
	push  eax
	cld
	mov   eax,dword[vesa_fontsmall_x_M2D]
	mov   cx,[ModeInfo_XResolution]
	sub   cx,10
	cmp   ax,cx
	jb    NoCR_inc_vesa_screen_pointerM2D
	jmp   inc_vesa_screen_pointerM2DExitError
NoCR_inc_vesa_screen_pointerM2D:
	add   eax,[FontW] 
	mov   dword[vesa_fontsmall_x_M2D],eax
inc_vesa_screen_pointerM2DExit:
	pop   eax
	pop   ecx
	clc
	ret

inc_vesa_screen_pointerM2DExitError:
	pop   eax
	pop   ecx
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintBitMapFonts_Direct                            ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;    ES:ESI points to asciiz string (zero ended).    ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ;       EDX color xxRRGGBB                           ;
 ;       ECX 1 = 8x8fonts                             ;
 ;           2 = 8x16fonts                            ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintBitMapFonts_Direct:
	pushad
	push  es
	mov   [FontColor],edx
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	mov   [FontColor],edx
	cmp   ecx,1
	jne   Try8x16fonts_Direct
	call  print_vesa_string_small8x8M2D_Direct
	jc    PrintBitMapFontsExitError_Direct
	jmp   PrintBitMapFontsExit_Direct
Try8x16fonts_Direct:
	cmp   ecx,2
	jne   PrintBitMapFontsExitError_Direct
	call  print_vesa_string_small8x16M2D_Direct
	jc    PrintBitMapFontsExitError_Direct
PrintBitMapFontsExit_Direct:
	pop   es
	popad
	clc
	ret

PrintBitMapFontsExitError_Direct:
	pop   es
	popad
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintBitMapFonts_Direct_BGColor                    ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;    ES:ESI points to asciiz string (zero ended).    ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ;       EDX color xxRRGGBB                           ;
 ;       EDI background color xxRRGGBB                ;
 ;       ECX 1 = 8x8fonts                             ;
 ;           2 = 8x16fonts                            ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintBitMapFonts_Direct_BGColor:
	pushad
	push  es
	mov   [FontColor],edx
	mov   [FontColorBG],edi
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	mov   [FontColor],edx
	cmp   ecx,1
	jne   Try8x16fonts_Direct_BGColor
	call  print_vesa_string_small8x8M2D_Direct_BGColor
	jc    PrintBitMapFontsExitError_Direct_BGColor
	jmp   PrintBitMapFontsExit_Direct_BGColor
Try8x16fonts_Direct_BGColor:
	cmp   ecx,2
	jne   PrintBitMapFontsExitError_Direct_BGColor
	call  print_vesa_string_small8x16M2D_Direct_BGColor
	jc    PrintBitMapFontsExitError_Direct_BGColor
PrintBitMapFontsExit_Direct_BGColor:
	pop   es
	popad
	clc
	ret

PrintBitMapFontsExitError_Direct_BGColor:
	pop   es
	popad
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; PrintBitMapFontsLen                                ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;    ES:ESI points to asciiz string (zero ended).    ;
 ;       EAX 32-17 = X                                ;
 ;           16- 1 = Y                                ;
 ;       EDX color xxRRGGBB                           ;
 ;       ECX 1 = 8x8fonts                             ;
 ;           2 = 8x16fonts                            ;
 ;       EDI   = String len                           ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintBitMapFontsLen_Direct:
	pushad
	push  es
	mov   [FontColor],edx
	mov   dword[vesa_fontsmall_x_M2D],eax
	shr   dword[vesa_fontsmall_x_M2D],16
	mov   dword[vesa_fontsmall_y_M2D],eax
	shl   dword[vesa_fontsmall_y_M2D],16
	shr   dword[vesa_fontsmall_y_M2D],16
	mov   [FontColor],edx
	cmp   ecx,1
	jne   Try8x16fonts_Direct
	call  print_vesa_string_small8x8M2DLen_Direct
	jc    PrintBitMapFontsExitErrorLen_Direct
	jmp   PrintBitMapFontsExitLen_Direct
Try8x16fontsLen_Direct:
	cmp   ecx,2
	jne   PrintBitMapFontsExitErrorLen_Direct
	call  print_vesa_string_small8x16M2DLen_Direct
	jc    PrintBitMapFontsExitErrorLen_Direct
PrintBitMapFontsExitLen_Direct:
	pop   es
	popad
	clc
	ret

PrintBitMapFontsExitErrorLen_Direct:
	pop   es
	popad
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x8fonts M2D ver _Direct        ;
 ;----------------------------------------------------;
print_vesa_string_small8x8M2D_Direct:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax			   ; width of buffer to add on
	mov   [FontW],6 				   ; width of font
	mov   byte [Row],9				   ; hight of font
	xor   eax,eax 
print_vesa_string_small8x8M2DLoop_Direct:
	mov   al,[esi]
	mov   [tchar],al				   ; save font
	cmp   al,0					   ; 0   = end of string
	je    print_vesa_string_small8x8M2DExit_Direct
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D1_Direct
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D1_Direct
	call  Convert_Text_Small8M2D_Direct
	jc    print_vesa_string_small8x8M2DExitError_Direct
Number2Small_BigM2D1_Direct:
	inc   esi
	jmp   print_vesa_string_small8x8M2DLoop_Direct
print_vesa_string_small8x8M2DExit_Direct:
	clc
	ret
print_vesa_string_small8x8M2DExitError_Direct:
	stc
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts M2D ver _Direct       ;
 ;----------------------------------------------------;
print_vesa_string_small8x16M2D_Direct:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],9
	mov   byte [Row],16
	xor   eax,eax 
print_vesa_string_small8x16M2DLoop_Direct:
	mov   al,[esi]
	mov   [tchar],al
	cmp   al,0					   ; 0x00 = end of string
	je    print_vesa_string_small8x16M2DExit_Direct
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D2_Direct
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D2_Direct
	call  Convert_Text_Small16M2D_Direct
	jc    print_vesa_string_small8x16M2DExitError_Direct
Number2Small_BigM2D2_Direct:
	inc   esi
	jmp   print_vesa_string_small8x16M2DLoop_Direct
print_vesa_string_small8x16M2DExit_Direct:
	clc
	ret
print_vesa_string_small8x16M2DExitError_Direct:
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x8fonts M2D ver _Direct _BGColor           
 ;----------------------------------------------------;
print_vesa_string_small8x8M2D_Direct_BGColor:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax			   ; width of buffer to add on
	mov   [FontW],6 				   ; width of font
	mov   byte [Row],9				   ; hight of font
	xor   eax,eax 
print_vesa_string_small8x8M2DLoop_Direct_BGColor:
	mov   al,[esi]
	mov   [tchar],al				   ; save font
	cmp   al,0					   ; 0   = end of string
	je    print_vesa_string_small8x8M2DExit_Direct_BGColor
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D1_Direct_BGColor
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D1_Direct_BGColor
	call  Convert_Text_Small8M2D_Direct_BGColor
	jc    print_vesa_string_small8x8M2DExitError_Direct_BGColor
Number2Small_BigM2D1_Direct_BGColor:
	inc   esi
	jmp   print_vesa_string_small8x8M2DLoop_Direct_BGColor
print_vesa_string_small8x8M2DExit_Direct_BGColor:
	clc
	ret
print_vesa_string_small8x8M2DExitError_Direct_BGColor:
	stc
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts M2D ver _Direct _BGColor             
 ;----------------------------------------------------;
print_vesa_string_small8x16M2D_Direct_BGColor:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],8 ;9
	mov   byte [Row],16
	xor   eax,eax 
print_vesa_string_small8x16M2DLoop_Direct_BGColor:
	mov   al,[esi]
	mov   [tchar],al
	cmp   al,0					   ; 0x00 = end of string
	je    print_vesa_string_small8x16M2DExit_Direct_BGColor
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D2_Direct_BGColor
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D2_Direct_BGColor
	call  Convert_Text_Small16M2D_Direct_BGColor
	jc    print_vesa_string_small8x16M2DExitError_Direct_BGColor
Number2Small_BigM2D2_Direct_BGColor:
	inc   esi
	jmp   print_vesa_string_small8x16M2DLoop_Direct_BGColor
print_vesa_string_small8x16M2DExit_Direct_BGColor:
	clc
	ret
print_vesa_string_small8x16M2DExitError_Direct_BGColor:
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x8fonts M2D ver  Len _Direct   ;
 ;----------------------------------------------------;
print_vesa_string_small8x8M2DLen_Direct:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   eax,8*4
	mov   dword[FontAddOn],eax			   ; width of buffer to add on
	mov   [FontW],6 				   ; width of font
	mov   byte [Row],9				   ; hight of font
	xor   eax,eax 
	mov   ecx,edi					   ; string count
print_vesa_string_small8x8M2DLoopLen_Direct:
	mov   al,[esi]
	mov   [tchar],al				   ; save font
	;cmp   al,0                                        ; 0   = end of string
	;je    print_vesa_string_small8x8M2DExitLen_Direct
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D1Len_Direct
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D1Len_Direct
	call  Convert_Text_Small8M2D_Direct
	jc    print_vesa_string_small8x8M2DExitErrorLen_Direct
Number2Small_BigM2D1Len_Direct:
	inc   esi
	loop   print_vesa_string_small8x8M2DLoopLen_Direct
print_vesa_string_small8x8M2DExitLen_Direct:
	clc
	ret
print_vesa_string_small8x8M2DExitErrorLen_Direct:
	stc
	ret


 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints vesa string 8x16fonts M2D ver  Len          ;
 ;----------------------------------------------------;
print_vesa_string_small8x16M2DLen_Direct:
	xor   eax,eax
	mov   ax,[ModeInfo_XResolution]
	shl   eax,2
	sub   ax,8*4
	mov   dword[FontAddOn],eax
	mov   [FontW],9
	mov   byte [Row],16
	xor   eax,eax 
	mov   ecx,edi					   ; string count
print_vesa_string_small8x16M2DLoopLen_Direct:
	mov   al,[esi]
	mov   [tchar],al
	;cmp   al,0                                        ; 0x00 = end of string
	;je    print_vesa_string_small8x16M2DExitLen_Direct
	cmp   al,32					   ; under 32 = too small number
	jb    Number2Small_BigM2D2Len_Direct
	cmp   al,128					   ; over 128 = too big a number
	ja    Number2Small_BigM2D2Len_Direct
	call  Convert_Text_Small16M2D_Direct
	jc    print_vesa_string_small8x16M2DExitErrorLen_Direct
Number2Small_BigM2D2Len_Direct:
	inc   esi
	loop   print_vesa_string_small8x16M2DLoopLen_Direct
print_vesa_string_small8x16M2DExitLen_Direct:
	clc
	ret
print_vesa_string_small8x16M2DExitErrorLen_Direct:
	stc
	ret

 ;--------------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x8 fonts M2D ver. _Direct ;
 ;--------------------------------------------------------------------;
Convert_Text_Small8M2D_Direct:
	push  ecx 
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	lea   esi,[Dex8x8+eax*9]  
	call  get_vesa_screen_pointerM2D_Direct
	call  Draw_Char_small32_Direct
	call  inc_vesa_screen_pointerM2D_Direct
	jc    Convert_Text_Small8M2DExitError_Direct
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	clc
	ret

Convert_Text_Small8M2DExitError_Direct:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;---------------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x16 fonts M2D ver. _Direct ;
 ;---------------------------------------------------------------------;
Convert_Text_Small16M2D_Direct:
	push  ecx
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	shl   eax,4
	lea   esi,[Dex8x16+eax] 
	call  get_vesa_screen_pointerM2D_Direct
	call  Draw_Char_small32_Direct
	call  inc_vesa_screen_pointerM2D_Direct
	jc    Convert_Text_Small16M2DExitError_Direct
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	ret

Convert_Text_Small16M2DExitError_Direct:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;--------------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x8 fonts M2D ver. _Direct ;
 ;--------------------------------------------------------------------;
Convert_Text_Small8M2D_Direct_BGColor:
	push  ecx 
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	lea   esi,[Dex8x8+eax*9]  
	call  get_vesa_screen_pointerM2D_Direct
	call  Draw_Char_small32_Direct_BGColor
	call  inc_vesa_screen_pointerM2D_Direct
	jc    Convert_Text_Small8M2DExitError_Direct_BGColor
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	clc
	ret

Convert_Text_Small8M2DExitError_Direct_BGColor:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;---------------------------------------------------------------------;
 ; Converts a string from text to vesa mode8x16 fonts M2D ver. _Direct ;
 ;---------------------------------------------------------------------;
Convert_Text_Small16M2D_Direct_BGColor:
	push  ecx
	push  ebx
	push  eax
	push  esi
	xor   eax,eax
	mov   al,[tchar]
	shl   eax,4
	lea   esi,[Dex8x16+eax] 
	call  get_vesa_screen_pointerM2D_Direct
	call  Draw_Char_small32_Direct_BGColor
	call  inc_vesa_screen_pointerM2D_Direct
	jc    Convert_Text_Small16M2DExitError_Direct_BGColor
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	ret

Convert_Text_Small16M2DExitError_Direct_BGColor:
	pop   esi
	pop   eax
	pop   ebx
	pop   ecx
	stc
	ret

 ;-----------------------------------------------------------;
 ; gets vesa screen pointer M2D ver.                         ;
 ;-----------------------------------------------------------;
get_vesa_screen_pointerM2D_Direct:
	push  eax					   
	push  ebx
	xor   eax,eax
	xor   ebx,ebx
	mov   bx,[ModeInfo_XResolution] 
	shl   ebx,2
	mov   [BytesPerScanLine],ebx
	mov   eax,dword[vesa_fontsmall_x_M2D]
	mov   ebx,dword[vesa_fontsmall_y_M2D]
	imul  ebx,[BytesPerScanLine]			      ; ebx = y * y multiplier
	lea   edi,[ebx+eax*4]				      ; edi = x*4+(y*y multiplier)
	add   edi,dword[ModeInfo_PhysBasePtr]		      ; ebx = where pixel is in memory
	pop   ebx
	pop   eax
	ret

 ;----------------------------------------------------;
 ; inc_vesa_screen_pointer _Direct.                   ;
 ;----------------------------------------------------;
inc_vesa_screen_pointerM2D_Direct:
	push  ecx
	push  eax
	cld
	mov   eax,dword[vesa_fontsmall_x_M2D]
	mov   cx,[ModeInfo_XResolution]
	sub   cx,10
	cmp   ax,cx
	jb    NoCR_inc_vesa_screen_pointerM2D_Direct
	jmp   inc_vesa_screen_pointerM2DExitError_Direct
NoCR_inc_vesa_screen_pointerM2D_Direct:
	add   eax,[FontW] 
	mov   dword[vesa_fontsmall_x_M2D],eax
inc_vesa_screen_pointerM2DExit_Direct:
	pop   eax
	pop   ecx
	clc
	ret

inc_vesa_screen_pointerM2DExitError_Direct:
	pop   eax
	pop   ecx
	stc
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Draw Char small32bpp ; draws char from al in vesa. ;
 ; _Direct                                            ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      Al Ascii character to write.                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;

Draw_Char_small32_Direct:
	push  es
	pushad
	mov   ax,8h
	mov   es,ax
	xor   edx,edx
	xor   ebx,ebx
	xor   eax,eax
	xor   ecx,ecx
	mov   dl,[Row]	
	cld
GetData32_Direct:	       
	lodsb 
	shl   ax,8		    
	mov   bx,ax	     
	mov   cl,8	    
CheckBit32_Direct:		   
	test  bx,8000h	      
	jz    ZeroBit32_Direct 
	push  eax      
	mov   eax,[FontColor]  
	stosd
	pop   eax
	jmp   Skip32_Direct		
ZeroBit32_Direct:
	add   edi,4
Skip32_Direct:		
	shl   bx,1	      
	loop  CheckBit32_Direct  
	add   edi,dword[FontAddOn] 
	dec   dl
	jnz   GetData32_Direct
	popad
	pop   es
	ret

 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Draw Char small32bpp ; draws char from al in vesa. ;
 ; _Direct                                            ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      Al Ascii character to write.                  ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;

Draw_Char_small32_Direct_BGColor:
	push  es
	pushad
	mov   ax,8h
	mov   es,ax
	xor   edx,edx
	xor   ebx,ebx
	xor   eax,eax
	xor   ecx,ecx
	mov   dl,[Row]	
	cld
GetData32_Direct_BGColor:	       
	lodsb 
	shl   ax,8		    
	mov   bx,ax	     
	mov   cl,8	    
CheckBit32_Direct_BGColor:		   
	test  bx,8000h	      
	jz    ZeroBit32_Direct_BGColor 
	push  eax      
	mov   eax,[FontColor]  
	stosd
	pop   eax
	jmp   Skip32_Direct_BGColor		
ZeroBit32_Direct_BGColor:
	push  eax      
	mov   eax,[FontColorBG]  
	stosd
	pop   eax
Skip32_Direct_BGColor:		
	shl   bx,1	      
	loop  CheckBit32_Direct_BGColor  
	add   edi,dword[FontAddOn] 
	dec   dl
	jnz   GetData32_Direct_BGColor
	popad
	pop   es
	ret

 ;----------------------------------------------------;
 ; DATA.                                              ;
 ;----------------------------------------------------;
vesa_fontsmall_x_M2D   dd 0			     
vesa_fontsmall_y_M2D   dd 0 
vesa_fontsmall_x       dd 0			     
vesa_fontsmall_y       dd 0			     
textx		       dw 0
texty		       dw 0
Row		       db 0
tcolor		       db 0
FontColor	       dd 0
FontColorBG	       dd 0
FontAddOn	       dd 0
tchar		       db 0
color		       dd 0  
red		       dd 0
blue		       dd 0  
bpp		       dd 0
FontCopy	       dw 0
FontW		       dd 0
chel		       db 0

