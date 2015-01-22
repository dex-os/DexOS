;=========================================================;
; Edit inc.                                      30/03/11 ;
;---------------------------------------------------------;
; DexOS OS V0.03                                          ;
; by Craig Bamford.                                       ;
;                                                         ;
;=========================================================;

;====================================================;
; GetChar.                                           ;
;====================================================;
GetChar:  
	push  ax
	pushf
	call  [WaitForKeyPress]  
	mov   byte[Char],ah
	popf
	pop   ax
	ret

;====================================================;
; GetKey.                                            ;
;====================================================;
GetKey:   
	push  ax
	pushf
	call  [WaitForKeyPress]
	mov   byte[ScanKey],ah
	mov   byte[AsciiKey],al
	popf
	pop   ax
	ret

;====================================================;
; Cursor_1.                                          ;
;====================================================;
Cursor_1:
	push  cx
	mov   ch,0x0b ;00
	mov   cl,0x0c ;14
	mov   ah,1
	call  [RealModeInt10h]
	pop   cx
	ret  

;====================================================;
; Cursor_0.                                          ;
;====================================================;
Cursor_0:
	push  cx
	mov   ch,0x1c
	mov   cl,0x1b
	mov   ah,1
	call  [RealModeInt10h]
	pop   cx
	ret  

;====================================================;
; GoToXYBios.                             ;AL=X,AH=Y ;
;====================================================;
GoToXYDex:
	mov   al,byte[PosX]
	mov   ah,byte[PosY]
	call  [SetCursorPos]
	ret

;====================================================;
; GoToXY.                                            ;
;====================================================;
GoToXY:
	pushad
	mov	dl,byte[PosX]
	mov	dh,byte[PosY]
	mov	al,80
	mul	dh
	xor	dh,dh
	add	ax,dx
	mov	cx,ax
	mov	dx,0x03d4
	mov	al,0x0e
	out	dx,al
	inc	dx
	mov	al,ch
	out	dx,al
	mov	dx,0x3d4
	mov	al,0x0f
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	popad
	ret

;====================================================;
; GoToXY.                                            ;
;====================================================;
GoToXYTexWin:
	pushad
	mov	al,80
	mul	dh
	xor	dh,dh
	add	ax,dx
	mov	cx,ax
	mov	dx,0x03d4
	mov	al,0x0e
	out	dx,al
	inc	dx
	mov	al,ch
	out	dx,al
	mov	dx,0x3d4
	mov	al,0x0f
	out	dx,al
	inc	dx
	mov	al,cl
	out	dx,al
	popad
	ret
;====================================================;
; SpeedUpKeyBoard.                                   ;
;====================================================;

SpeedUpKeyBoard:
	push  eax
	mov   al,0xf3				     
	out   0x60,al				      
KeyBoardWait:
	in    al,0x64
	test  al,10b				      
	jne   KeyBoardWait			      
	mov   al,1 
	and   al,111b
	out   0x60,al				      
	pop   eax
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
	mov   ax,[CursorX]
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

;====================================================;
; ConvertYtoDec.                                     ;
;====================================================;
ConvertYtoDec:
	pushad
	push  es
	push  ds
	mov   word[ConVarY],ax
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   dword[StringAddess],esi
	mov   esi,dword[StringAddess]
	mov   dl,' '
	mov   byte[ds:esi],dl
	mov   byte[ds:esi+1],dl
	mov   byte[ds:esi+2],dl
	mov   byte[ds:esi+3],dl
	mov   byte[ds:esi+4],dl
	mov   ax,word[ConVarY]
	mov   bx,10
loopd5: 
	mov   dx,0
	div   bx
	add   dl,30h
	mov   cl,byte[ds:esi+3]
	mov   byte[ds:esi+4],cl
	mov   cl,byte[ds:esi+2]
	mov   byte[ds:esi+3],cl
	mov   cl,byte[ds:esi+1]
	mov   byte[ds:esi+2],cl
	mov   cl,byte[ds:esi]
	mov   byte[ds:esi+1],cl
	mov   byte [ds:esi],dl
	cmp   ax,0
	jne   loopd5
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Convt HextoAsc.            :Input=AX,Output=AscVal.;
;====================================================;
Convt:
	pushad
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   word[AscVal],0x2020
	mov   cx,10
	mov   esi,AscVal+1
	cmp   ax,cx
	jb    d20
	div   cl
	or    ah,0x30
	mov   byte[esi],ah
	dec   esi
d20:	or    al,0x30
	mov   byte[esi],al
	pop   ds
	popad
	ret

;====================================================;
; New.                                               ;
;====================================================;
New:
	call  SetBuffer
	call  NoFileName
	call  ClearBuffer
	mov   [FddSecLoadedCount],0
	mov   [EndOfFile],0
	mov   [LineCount],1 
	mov   edi,PageUpPointerAray1 
	mov   [PageUpPointer],edi 
	mov   [NumberOfLinesInFile],2
	mov   ebx,Buffer
	mov   [BufferAddOn1],ebx
	mov   [TopOfScreen],1
	mov   [ExitEditor],0
	mov   [WinY],4
	mov   [CursorX],1
	mov   [CursorY],1
	mov   [TotalLine],2
	mov   [ColStart],0 
	mov   [PosX],0 
	mov   [PosY],2 ;3 ;6
	ret
 ;----------------------------------------------------;
 ; Beep                                               ;
 ;----------------------------------------------------; 
Beep:
	pushad
	mov   [Hz],0x200 
	call  Sound
	mov   ax,3
	call  [SetDelay]
	call  NoSound
	popad
	ret
 ;----------------------------------------------------;
 ; Sound                                              ;
 ;----------------------------------------------------;
Sound:
	mov   bx,[Hz]
	mov   ax,0x34dd 	       
	mov   dx,0x0012 	       
	cmp   dx,bx		   
	jnc   Done1		  
	div   bx		
	mov   bx,ax
	in    al,0x61
	test  al,3
	jnz   A99		 
	or    al,3
	out   0x61,al		     
	mov   al,0xb6
	out   0x43,al
A99:	
	mov   al,bl 
	out   0x42,al		   
	mov   al,bh
	out   0x42,al
Done1:
	ret

 ;----------------------------------------------------;
 ; NoSound                                            ;
 ;----------------------------------------------------;
NoSound:
	in    al,0x61		      
	and   al,11111100b
	out   0x61,al		     
	ret 
	       
 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;
Char	      db   0
ScanKey       db   0
AsciiKey      db   0
AscVal	      dw   0
