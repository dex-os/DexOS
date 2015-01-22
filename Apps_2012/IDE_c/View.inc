;====================================================;
; Cls.                                               ;
;====================================================;
Cls:
	pushad
	push  es
	mov   ax,18h
	mov   es,ax
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+4*1]
	mov   ax,8h
	mov   es,ax
	mov   ah,color2
	mov   al,' '
	mov   cx,22*80
	rep   stosw
	pop   es
	popad
	ret


;====================================================;
; PrintPage.                                         ;
;====================================================;
PrintPage:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   ax,8h
	mov   es,ax
	mov   al,color2 		      ;cc
	mov   [FileBufferAddOn],0
	mov   esi,PageUpPointerAray 
	mov   edx,[TopOfScreen]
	shl   edx,8 
	add   esi,edx
	add   esi,[ColStart] 
	mov   edi,0xb8000
	add   edi,160*1
	mov   ah,color2
	xor   ecx,ecx
	mov   cx,80
	mov   al,' '
	rep   stosw
	mov   cx,22
PrintPageLoop:
	push  cx
	mov   cx,80
PrintPageLoop1:
	lodsb
	stosw
	loop  PrintPageLoop1
	mov   esi,PageUpPointerAray 
	mov   edx,[TopOfScreen]
	shl   edx,8
	add   esi,edx
	add   [FileBufferAddOn],256 
	add   esi,[FileBufferAddOn]
	add   esi,[ColStart] 
	pop   cx
	loop  PrintPageLoop
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; PrintLine.                                         ;
;====================================================;
PrintLine:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   ax,8h
	mov   es,ax
	mov   al,color2
	mov   esi,PageUpPointerAray 
	xor   edx,edx
	mov   dx,[CursorY]
	shl   edx,8
	add   esi,edx
	xor   eax,eax
	add   esi,[ColStart] 
	mov   edi,0xb8000
	mov   al,[PosY]
	mov   dl,160
	mul   dl
	add   edi,eax
	mov   ah,color2
	xor   ecx,ecx
	mov   cx,80
PrintLineLoop:
	lodsb
	stosw
	loop  PrintLineLoop
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; ClearBuffer.                                       ;
;====================================================;
ClearBuffer:
	pushad
	push  es
	mov   ax,18h
	mov   es,ax
	mov   edi,PageUpPointerAray 
	mov   ecx,60*256
	mov   al,' '
	rep   stosb
	pop   es
	popad
	ret

;====================================================;
; NumberOfLines.                                     ;
;====================================================;
NumberOfLines:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   [EOFtest],1
	mov   [FileBufferAddOn],0
	mov   ebx,Buffer		;get where the file is
	mov   eax,out_buffer2		;put it in 2nd counter
	xor   ecx,ecx			;0 line position
	xor   edi,edi
process4:
	mov   dl,[ebx]			;get char
	cmp   dl,09h			;is it tab?
	je    new_tabber4
	cmp   dl,0Ah			;is it linefeed?
	je    AddLine			;nothing4 
	cmp   dl,0Dh			;is it cariage return?
	je    nothing4			;AddLine;yeah, Add a Line
	cmp   dl,1Ah			;is it end of file?
	je    ClearTheBuffer 
	cmp   dl,0			;is it null?
	je    ClearTheBuffer 
	mov   [eax],dl			;if not, stick in buffer
	cmp   eax, out_buffer2 + 252	;check buffer, full? 
	jae   AddLine			;yeah, Add a Line
	inc   eax			;else next buffer offset
	inc   ebx			;next  char
	inc   ecx			;scrn pos inc
	jmp   process4			;loop
nothing4:
	inc   ebx
	jmp   process4
new_tabber4:
	mov   esi,ecx			;save the eax
	and   cl,7			;get the desired area
	cmp   cl,0			;chk for 0
	jz    simp_tab4 		;if 0 simple  tab
@@:	inc   esi			;incrememt esi
	inc   eax			;increment eax
	mov   ecx,esi			;reload ecx
	and   cl,7			;get desired area
	cmp   cl,0			;check if 0
	jz    @f			;if yeah keep spot
	jmp   @b			;if not, another round
@@:
	mov   ecx,esi
	inc   ebx
	jmp   process4 
simp_tab4:
	mov   ecx,esi
	add   eax,8
	add   ecx,8
	inc   ebx
	jmp   process4
ClearTheBuffer:
	mov   [EOFtest],0
	jmp   AddLine
NumberOfLinesEnd:
	mov   [NumberOfLinesInFile], edi
	mov   eax,[NumberOfLinesInFile] 
	mov   word[TotalLine],ax
	sub   eax,38
	mov   [EndOfTexFile],eax
	pop   ds
	pop   es
	popad
	ret			       ;end of prog.

;====================================================;
; AddLine.                                           ;
;====================================================;
AddLine:
	call  AddLineToFileBuffer
	mov   eax,out_buffer2	       ;put address in eax
	mov   ecx,300  
@@:
	mov   byte [eax],' '	       ;make buffer spaces
	inc   eax
	dec   ecx
	jnz   @b
	mov   eax,out_buffer2	       ;reset eax to buffer 0
	inc   ebx		       ;tell next char   ;CC
	xor   ecx,ecx		       ;0 the screen index
	inc   edi
	cmp   [EOFtest],0
	je    NumberOfLinesEnd
	cmp   edi,20000 
	jbe   process4		       ;if not, keep going
	jmp   NumberOfLinesEnd

;====================================================;
; AddLineToFileBuffer.                               ;
;====================================================;
AddLineToFileBuffer:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   esi,out_buffer2
	mov   edi,PageUpPointerAray1
	add   edi,[FileBufferAddOn]
	mov   cx,256  
	rep   movsb
	add   [FileBufferAddOn],256 
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; BackSpaceLine.                                     ;
;====================================================;
BackSpaceLine:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   esi,PageUpPointerAray 
	xor   edx,edx
	mov   dx,[CursorY]
	shl   edx,8			   ;mul edx by 256
	add   esi,edx
	mov   ecx,256  
	sub   cx,[CursorX]
	dec   cx
	xor   eax,eax
	mov   ax,[CursorX]
	add   esi,eax
	mov   edi,esi
	dec   edi
	cld
	rep   movsb
	dec    edi
	mov   al,' '
	stosb
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; AddLineToFile.                                     ;
;====================================================;
AddLineToFile:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   esi,PageUpPointerAray 
	mov   edx,[NumberOfLinesInFile]   
	inc   edx
	shl   edx,8			;mul edx by 256
	add   esi,edx
	dec   esi
	mov   edi,esi
	add   edi,256  
	mov   ecx,[NumberOfLinesInFile]
	sub   cx,[CursorY]
	shl   ecx,8			;mul ecx by 256
	std
	rep   movsb
	cld
	sub   edi,256  
	mov   al,' '
	mov   ecx,256  
	rep   stosb
	add   [NumberOfLinesInFile],1
	add   [TotalLine],1
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; DeleteLineInFile.                                  ;
;====================================================;
DeleteLineInFile:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   esi,PageUpPointerAray 
	mov   edx,[LineCount]	
	shl   edx,8			     ;mul edx by 256
	add   esi,edx
	mov   edi,esi
	add   esi,256
	mov   ecx,[NumberOfLinesInFile]
	sub   cx,[CursorY]
	shl   ecx,8  
	cld
	rep   movsb
	mov   al,' '
	mov   ecx,256  
	rep   stosb
	sub   [NumberOfLinesInFile],1
	sub   [TotalLine],1
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; GoToEndOfLine.                                     ;
;====================================================;
GoToEndOfLine:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   esi,PageUpPointerAray 
	xor   edx,edx
	mov   dx,[CursorY]
	shl   edx,8
	add   esi,edx
	mov   ecx,253	  
	add   esi,ecx
	inc   ecx 
	std
GoToEndOfLineLoop:
	lodsb
	cmp   al,' '
	jne   @f
	loop  GoToEndOfLineLoop
@@:	
	cld
	cmp   ecx,80
	jb    @f
	mov   [PosX],79
	inc   ecx
	mov   [CursorX],cx 
	sub   ecx,80 
	mov   [ColStart],ecx
	jmp   GoToEndOfLineJump
@@:	
	mov   [PosX],cl 
	mov   [ColStart],0 
	inc   ecx
	mov   [CursorX],cx 
GoToEndOfLineJump:
	call  PrintPage
	call  info_win
	call  GoToXY
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; WriteChar.                                         ;
;====================================================;
WriteChar:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   esi,PageUpPointerAray 
	xor   edx,edx
	mov   dx,[CursorY]
	shl   edx,8			 ;mul edx by 256
	add   esi,edx
	mov   ecx,254  
	sub   cx,[CursorX]
	inc   cx
	add   esi,254  
	mov   edi,esi
	dec   esi
	cmp   byte[es:edi-2],' '
	jne   WriteCharEnd
	cmp   [CursorX],253  
	ja    WriteCharEnd
	std
	rep   movsb
	cld
	mov   al,[AsciiKey] 
	stosb
	cmp   [CursorX],254  
	je    NoScreenMovWC
	inc   [CursorX]
	call  info_win
	cmp   [PosX],79
	je    MovScreenWC 
	inc   [PosX]
	call  GoToXY  
	jmp   NoScreenMovWC 
MovScreenWC: 
	inc   [ColStart]
NoScreenMovWC: 
	call  PrintPage
	call  info_win
	;call  CurrantDir
WriteCharEnd:
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; MoveLine.                                          ;
;====================================================;
MoveLine:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   esi,PageUpPointerAray 
	xor   edx,edx
	mov   dx,[CursorY]
	shl   edx,8				;mul edx by 256
	add   esi,edx
	mov   edi,esi
	mov   ecx,254 
	sub   cx,[CursorX]
	inc   cx
	push  ecx
	xor   edx,edx
	mov   dx,[CursorX]
	add   esi,edx
	dec   esi
	push  esi
	add   edi,256  
	rep   movsb
	pop   edi
	pop   ecx
	mov   al,' '
	rep   stosb
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; EndOfLine.                                         ;
;====================================================;
EndOfLine:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   esi,PageUpPointerAray 
	xor   edx,edx
	mov   dx,[CursorY]
	shl   edx,8		       ;mul edx by 256
	add   esi,edx
	sub   esi,[EndOfLineAddOn]
	mov   ecx,253  
	add   esi,ecx
	inc   ecx 
	std
EndOfLineLoop:
	lodsb
	cmp   al,' '
	jne   @f
	loop  EndOfLineLoop
@@:	
	cld
	mov   [EOLNumber],ecx
	mov   [EndOfLineAddOn],0
	pop   ds
	pop   es
	popad
	ret
;====================================================;
; BackOneLine.                                       ;
;====================================================;
BackOneLine:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   esi,PageUpPointerAray 
	xor   edx,edx
	mov   dx,[CursorY]
	shl   edx,8			   ;mul edx by 256
	add   esi,edx
	mov   edi,esi
	sub   edi,256  
	add   edi,[EOLNumber]
	rep   movsb
	pop   ds
	pop   es
	popad
	ret

;----------------------------------------;
;               Data  Area               ;
;----------------------------------------;
out_buffer:
	times 300 db ' '
		  dd 0
out_buffer1:
	times 300 db ' '
		  dd 0
out_buffer2:
	times 300 db ' '
		  dd 0
