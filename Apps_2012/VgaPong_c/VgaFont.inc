
 ;'''''''''''''''''''''''''''''''''''''''''''''''''''';
 ; Prints string mode 13h.                            ;
 ;----------------------------------------------------;
 ;                                                    ;
 ;  Input:                                            ;
 ;      es:esi points to asciiz string to write.      ;
 ;                                                    ;
 ; Output:                                            ;
 ;      None.                                         ;
 ;....................................................;
PrintVGAstringSmall:
	push  eax
	push  esi
	mov   [VgaX],ax
	mov   [VgaY],bx
PrintVGAtextLoop:
	mov   al,[esi]
	mov   [VGAchar],al
	cmp   al,0					   
	je    VGAtextExit
	cmp   al,30					   
	jb    VGAtextExit 
	call  ConvertVGAtextSmall
@@:	inc   esi
	jmp   PrintVGAtextLoop
VGAtextExit:
	pop   esi
	pop   eax
	ret
 ;----------------------------------------------------;
 ; Put big Text                                       ;
 ;----------------------------------------------------;
GetVGAscreenPointer:
	push  ecx
	xor   ecx,ecx 
	mov   edi,ScreenBuffer 
	mov   cx,[VgaX]
	mov   bx,[VgaY]
	mov   dx,[VgaY] 	       
	shl   bx,8		    
	shl   dx,6		   
	add   bx,dx
	add   cx,bx		    
	add   edi,ecx	
	pop   ecx
	ret

 ;----------------------------------------------------;
 ; Converts a string from text to vesa mode.          ;
 ;----------------------------------------------------;
ConvertVGAtextSmall: 
	push  eax
	push  esi
	mov   al,[VGAchar]
	call  GetVGAtext
	mov   byte[Row],7
	call  GetVGAscreenPointer
	call  DrawImage4
	add   [VgaX],7
	pop   esi
	pop   eax
	ret
 ;----------------------------------------------------;
 ; DrawImage4                                         ;
 ;----------------------------------------------------;

DrawImage4:	       
	mov   dl,[Row] 
	cld	     
GetData14:  
	lodsb
	shl   ax,8	     
	mov   bx,ax		    
	mov   cl,8	   
CheckBit14:  
	test  bx,0x8000 	     
	jz    ZeroBit14  
	mov   al,[VGAcolor]    
	stosb				
	jmp   Skip14		    
ZeroBit14:	       
	inc   edi	    
Skip14:  
	shl   bx,1		 
	loop  CheckBit14	  
	add   edi,312	     
	dec   dl       
	jnz   GetData14 	    
	ret
 ;----------------------------------------------------;
 ; Put big Text                                       ;
 ;----------------------------------------------------;
PutTextB:
	mov   [Color],0x4A  
	mov   [Row],16
	mov   edi,ScreenBuffer 
	add   edi,716
	mov   esi,TextS
	
	call  DrawImage1
	mov   edi,ScreenBuffer	
	add   edi,733
	mov   esi,TextP
       
	call  DrawImage1
	mov   edi,ScreenBuffer	
	add   edi,750
	mov   esi,TextA
       
	call  DrawImage1 
	mov   edi,ScreenBuffer	   
	add   edi,767
	mov   esi,TextC
	
	call  DrawImage1 
	mov   edi,ScreenBuffer 
	add   edi,784
	mov   esi,TextE
       
	call  DrawImage1 
	mov   edi,ScreenBuffer 
	add   edi,813
	mov   esi,TextP
       
	call  DrawImage1
	mov   edi,ScreenBuffer	    
	add   edi,830
	mov   esi,TextO
      
	call  DrawImage1
	mov   edi,ScreenBuffer 
	add   edi,847
	mov   esi,TextN
       
	call  DrawImage1
	mov   edi,ScreenBuffer 
	add   edi,864
	mov   esi,TextG
       
	call  DrawImage1
	ret


;====================================================;
; get_text.                                          ;
;====================================================;
GetVGAtext:
	cmp   al,"A"
	jne   z1
	mov   esi,LetterA
	jmp   endd
z1:
	cmp   al,"B"
	jne   z2
	mov   esi,LetterB
	jmp   endd
z2:
	cmp   al,"C"
	jne   z3
	mov   esi,LetterC
	jmp   endd
z3:
	cmp   al,"D"
	jne   z4
	mov   esi,LetterD
	jmp   endd
z4:
	cmp   al,"E"
	jne   z5
	mov   esi,LetterE
	jmp   endd
z5:
	cmp   al,"F"
	jne   z6
	mov   esi,LetterF
	jmp   endd
z6:
	cmp   al,"G"
	jne   z7
	mov   esi,LetterG
	jmp   endd
z7:
	cmp   al,"H"
	jne   z8
	mov   esi,LetterH
	jmp   endd
z8:
	cmp   al,"I"
	jne   z9
	mov   esi,LetterI
	jmp   endd
z9:
	cmp   al,"J"
	jne   z10
	mov   esi,LetterJ
	jmp   endd
z10:
	cmp   al,"K"
	jne   z11
	mov   esi,LetterK
	jmp   endd
z11:
	cmp   al,"L"
	jne   z12
	mov   esi,LetterL
	jmp   endd
z12:
	cmp   al,"M"
	jne   z13
	mov   esi,LetterM
	jmp   endd
z13:
	cmp   al,"N"
	jne   z14
	mov   esi,LetterN
	jmp   endd
z14:
	cmp   al,"O"
	jne   z15
	mov   esi,LetterO
	jmp   endd
z15:
	cmp   al,"P"
	jne   z16
	mov   esi,LetterP
	jmp   endd
z16:
	cmp   al,"Q"
	jne   z17
	mov   esi,LetterQ
	jmp   endd
z17:
	cmp   al,"R"
	jne   z18
	mov   esi,LetterR
	jmp   endd
z18:
	cmp   al,"S"
	jne   z19
	mov   esi,LetterS
	jmp   endd
z19:
	cmp   al,"T"
	jne   z20
	mov   esi,LetterT
	jmp   endd
z20:
	cmp   al,"U"
	jne   z21
	mov   esi,LetterU
	jmp   endd
z21:
	cmp   al,"V"
	jne   z22
	mov   esi,LetterV
	jmp   endd
z22:
	cmp   al,"W"
	jne   z23
	mov   esi,LetterW
	jmp   endd
z23:
	cmp   al,"X"
	jne   z24
	mov   esi,LetterX
	jmp   endd
z24:
	cmp   al,"Y"
	jne   z25
	mov   esi,LetterY
	jmp   endd
z25:
	cmp   al,"Z"
	jne   z26
	mov   esi,LetterZ
	jmp   endd
z26:
	cmp   al," "
	jne   z27
	mov   esi,Space
	jmp   endd
z27:
	cmp   al,"0"
	jne   z28
	mov   esi,Zero
	jmp   endd
z28:
	cmp   al,"1"
	jne   z29
	mov   esi,One
	jmp   endd
z29:
	cmp   al,"2"
	jne   z30
	mov   esi,Two
	jmp   endd
z30:
	cmp   al,"3"
	jne   z31
	mov   esi,Three
	jmp   endd
z31:
	cmp   al,"4"
	jne   z32
	mov   esi,Four
	jmp   endd
z32:
	cmp   al,"5"
	jne   z33
	mov   esi,Five
	jmp   endd
z33:
	cmp   al,"6"
	jne   z34
	mov   esi,Six
	jmp   endd
z34:
	cmp   al,"7"
	jne   z35
	mov   esi,Seven
	jmp   endd
z35:
	cmp   al,"8"
	jne   z36
	mov   esi,Eight
	jmp   endd
z36:
	cmp   al,"9"
	jne   z37
	mov   esi,Nine
	jmp   endd
z37:
	cmp   al,"."
	jne   z38
	mov   esi,Period
	jmp   endd
z38:
	cmp   al,"="
	jne   z39
	mov   esi,Equal
	jmp   endd
z39:
	cmp   al,"#"
	jne   z40
	mov   esi,CopyrightSymbol
	jmp   endd
z40:
	cmp   al,0x00
	jne   z41
	mov   esi,Zero
	jmp   endd
z41:
	cmp   al,0x01
	jne   z42
	mov   esi,One
	jmp   endd
z42:
	cmp   al,0x02
	jne   z43
	mov   esi,Two
	jmp   endd
z43:
	cmp   al,0x03
	jne   z44
	mov   esi,Three
	jmp   endd
z44:
	cmp   al,0x04
	jne   z45
	mov   esi,Four
	jmp   endd
z45:
	cmp   al,0x05
	jne   z46
	mov   esi,Five
	jmp   endd
z46:
	cmp   al,0x06
	jne   z47
	mov   esi,Six
	jmp   endd
z47:
	cmp   al,0x07
	jne   z48
	mov   esi,Seven
	jmp   endd
z48:
	cmp   al,0x08
	jne   z49
	mov   esi,Eight
	jmp   endd
z49:
	cmp   al,0x09
	jne   z50
	mov   esi,Nine
	jmp   endd
z50:

	mov  esi,Space
endd:
	ret

;====================================================;
;  Fonts.                                            ;
;====================================================;

Zero			DB	38h,44h,4ch,54h,64h,44h,38h
One			DB	10h,30h,10h,10h,10h,10h,38h
Two			DB	38h,44h,04h,18h,20h,40h,7ch
Three			DB	38h,44h,04h,18h,04h,44h,38h
Four			DB	44h,44h,44h,7ch,04h,04h,04h
Five			DB	7ch,40h,40h,78h,04h,44h,38h
Six			DB	38h,44h,40h,78h,44h,44h,38h
Seven			DB	7ch,04h,08h,10h,10h,10h,10h
Eight			DB	38h,44h,44h,38h,44h,44h,38h
Nine			DB	38h,44h,44h,3ch,04h,44h,38h

Equal			DB	00h,00h,7ch,00h,7ch,00h,00h
Period			DB	00h,00h,00h,00h,00h,00h,10h
CopyrightSymbol 	DB	0x7c,0x82,0xba,0xa2,0xba,0x82,0x7c  
 
Space			DB	00h,00h,00h,00h,00h,00h,00h
LetterA 		DB	38h,44h,44h,7ch,44h,44h,44h
LetterB 		DB	78h,44h,44h,78h,44h,44h,78h
LetterC 		DB	38h,44h,40h,40h,40h,44h,38h
LetterD 		DB	78h,44h,44h,44h,44h,44h,78h
LetterE 		DB	7ch,40h,40h,78h,40h,40h,7ch
LetterF 		DB	7ch,40h,40h,78h,40h,40h,40h
LetterG 		DB	38h,44h,40h,5ch,44h,44h,38h
LetterH 		DB	44h,44h,44h,7ch,44h,44h,44h
LetterI 		DB	7ch,10h,10h,10h,10h,10h,7ch
LetterJ 		DB	04h,04h,04h,04h,04h,44h,38h
LetterK 		DB	44h,48h,50h,60h,50h,48h,44h
LetterL 		DB	40h,40h,40h,40h,40h,40h,7ch
LetterM 		DB	44h,6ch,54h,44h,44h,44h,44h
LetterN 		DB	44h,64h,54h,4ch,44h,44h,44h
LetterO 		DB	38h,44h,44h,44h,44h,44h,38h
LetterP 		DB	78h,44h,44h,78h,40h,40h,40h
LetterQ 		DB	38h,44h,44h,44h,44h,4ch,3ch
LetterR 		DB	78h,44h,44h,78h,44h,44h,44h
LetterS 		DB	38h,44h,40h,38h,04h,44h,38h
LetterT 		DB	7ch,10h,10h,10h,10h,10h,10h
LetterU 		DB	44h,44h,44h,44h,44h,44h,38h
LetterV 		DB	44h,44h,44h,44h,44h,28h,10h
LetterW 		DB	44h,44h,44h,44h,54h,6ch,44h
LetterX 		DB	44h,44h,28h,10h,28h,44h,44h
LetterY 		DB	44h,44h,28h,10h,10h,10h,10h
LetterZ 		DB	7ch,04h,08h,10h,20h,40h,7ch

VGAchar 		db 0
VgaY			dw 0
VgaX			dw 0
VGAcolor		db 0