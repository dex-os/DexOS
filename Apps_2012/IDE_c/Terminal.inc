 ;----------------------------------------------------;
 ; PALETTE                                            ;
 ;----------------------------------------------------;

Palette:
	pushad
	mov   dx,3c8h					 
	out   dx,al
	inc   dx					
	mov   al,bl					 
	out   dx,al
	mov   al,bh					 
	out   dx,al
	mov   al,cl					 
	out   dx,al
	popad
	ret

 ;----------------------------------------------------;
 ; Set a nifty color palette                          ;
 ;----------------------------------------------------;

SetPalette:
	mov   al,2
	mov   bx,3917 ;0x3f3f
	mov   cl,77  ;43
	call  Palette					    
	mov   al,4
	mov   cl,63
	call  Palette					      
	mov   al,5
	mov   bx,0x3228
	call  Palette					       
	mov   al,7
	call  Palette				       
	mov   al,5
	mov   bx,0x0032
	mov   cl,0
	call  Palette	 
	mov   al,58
	mov   bx,0x0A00
	call  Palette	   
	inc   al
	mov   bh,40
	call  Palette	
	mov   al,56
	mov   bx,0x1010
	mov   cl,32
	call  Palette  
	mov   al,60
	mov   bx,0x1818
	mov   cl,48
	call  Palette  
	ret

;====================================================;
; Topbar.                                            ;
;====================================================;
InitTopBar:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	mov   esi,TopBarMem
	mov   ah,0x0c ;0x09  
	mov   ecx,80*3	  
ClearTopBarLoop:  
	mov   al,[ds:esi]
	stosw
	inc   esi
	loop  ClearTopBarLoop
	pop   es
	popad
	ret 

;====================================================;
; Topborder.                                         ;
;====================================================;
TopBorder:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	add   edi,160*1 ;4
	mov   esi,BorderTop
	mov   ah,0x88 ;0x09
	mov   ecx,80*2	  
TopBorderLoop:	
	mov   al,[ds:esi]
	stosw
	inc   esi
	loop  TopBorderLoop
	pop   es
	popad
	ret 

;====================================================;
; Midborder.                                         ;
;====================================================;
;MidBorder:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	add   edi,160*3 
	mov   cx,41
	mov   ah,0x0c 
@@: 
	push  cx
	mov   ecx,80
	mov   esi,BorderMid    
MidBorderLoop1:  
	mov   al,[ds:esi]
	stosw
	inc   esi
	loop  MidBorderLoop1
	pop   cx
	loop  @b
	pop   es
	popad
	ret 

;====================================================;
; FullWhiteBar.                                      ;
;====================================================;
TopGreenBar:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	mov   cx,80
	mov   ah,0x2f
	mov   al,' '
	rep   stosw
	pop   es
	popad
	ret 

;====================================================;
; TopMenuBar.                                        ;
;====================================================;
TopMenuBar:
	pushad
	push  es
	mov   [WinY],4
	mov   [WinX],1
	mov   [BarExit],0
	mov   [MenuBar],1
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
NotAlt:
	call  GetKey
	call  LorR 
	cmp   al,0
	je    NoLorR
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	jmp   NotAlt
NoLorR:
	cmp   [ScanKey],Alt
	je    TopMenuBarExit 
	cmp   [AsciiKey],'f'
	jne   TryNext1
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],1
	call  WinHeadSetColorreversed
	call  DrawFile
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext1:
	cmp   [AsciiKey],'F'
	jne   TryNext2
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],1
	call  WinHeadSetColorreversed
	call  DrawFile
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext2:
	cmp   [AsciiKey],'e'
	jne   TryNext3
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],2
	call  WinHeadSetColorreversed
	call  DrawEdit
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext3:
	cmp   [AsciiKey],'E'
	jne   TryNext4
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],2
	call  WinHeadSetColorreversed
	call  DrawEdit
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext4:
	cmp   [AsciiKey],'s'
	jne   TryNext5
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],3
	call  WinHeadSetColorreversed
	call  DrawSearch
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext5:
	cmp   [AsciiKey],'S'
	jne   TryNext6
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],3
	call  WinHeadSetColorreversed
	call  DrawSearch
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit
	jmp   LorRpressed
TryNext6:
	cmp   [AsciiKey],'h'
	jne   TryNext7
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],4
	call  WinHeadSetColorreversed
	call  DrawHelp
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext7:
	cmp   [AsciiKey],'H'
	jne   TryNext8
	mov   [MenuBar],0
	call  WinHeadSetColor
	mov   [WinX],4
	call  WinHeadSetColorreversed
	call  DrawHelp
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext8:
	cmp   [AsciiKey],enter_a
	jne   TryNext9
LorRpressed:
	cmp   [WinX],1
	jne   WinXisNot1
	mov   [MenuBar],0
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	call  DrawFile
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
WinXisNot1:
	cmp   [WinX],2
	jne   WinXisNot2
	mov   [MenuBar],0
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	call  DrawEdit
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
WinXisNot2:
	cmp   [WinX],3
	jne   WinXisNot3
	mov   [MenuBar],0
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	call  DrawSearch
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
WinXisNot3:
	mov   [MenuBar],0
	call  WinHeadSetColor
	call  WinHeadSetColorreversed
	call  PrintPage 
	call  DrawHelp
	call  PrintPage 
	cmp   [BarExit],0
	jne   TopMenuBarExit 
	jmp   LorRpressed
TryNext9:
	jmp   NotAlt
TopMenuBarExit:
	mov   [MenuBar],0
	call  WinHeadSetColor
	pop   es
	popad
	ret 

;====================================================;
; WinHeadSetColor.                                   ;
;====================================================;
WinHeadSetColor:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	mov   esi,WinHead1
	add   edi,2
	mov   al,0x2f
	xor   ecx,ecx
	mov   cx,38 
loopWinHeadSetColor1: 
	movsb
	stosb
	loop  loopWinHeadSetColor1
	mov   [color4],0x2e
	cmp   [MenuBar],1
	je    @f
	mov   [color4],0x2f
@@:
	mov   edi,0xb8000
	add   edi,8
	mov   ah,[color4]
	mov   al,'F'
	stosw
	mov   edi,0xb8000
	add   edi,26
	mov   ah,[color4]
	mov   al,'E'
	stosw
	mov   edi,0xb8000
	add   edi,44
	mov   ah,[color4]
	mov   al,'S'
	stosw
	mov   edi,0xb8000
	add   edi,66
	mov   ah,[color4]
	mov   al,'H'
	stosw
	pop   es
	popad
	ret 

;====================================================;
; MediumWhiteBar.                                    ;
;====================================================;
BottomGreenBar:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	add   edi,160*24
	mov   cx,80
	mov   ah,0x2f
	mov   al,' '
	rep   stosw
	pop   es
	popad
	ret 

;====================================================;
; DrawFile.                                          ;
;====================================================;
DrawFile:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,4 
	mov   ecx,8
loopfShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,7
	mov   ecx,15
	mov   al,Shadow1
rev_fShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_fShadow
	pop   ecx
	loop  loopfShadow
	mov   [WinY],1
	cld
 ;----------------------------------------------------;
 ; Draw File Window.                                  ;
 ;----------------------------------------------------;
draw_f: 
	mov   esi,WinFile+3			      
	xor   ecx,ecx
	mov   cl,[WinFile+1]
	xor   ebx,ebx
	mov   bx,0
loopf2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,4
	push  ecx
	xor   ecx,ecx
	mov   al,[WinFile+2]
	mov   cl,[WinFile]
loopf1: 
	movsb
	stosb
	loop  loopf1
	pop   ecx
	loop  loopf2
 ;----------------------------------------------------;
 ; Mark Selection.                                    ;
 ;----------------------------------------------------;
	mov   bx,[WinY] 	    
	shl   bx,2 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4]
	add   edi,7
	xor   cx,cx
	mov   cl,[WinFile]
	sub   cl,2
	mov   al,0x87 ;reverseMark
rev_f:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_f
get_f: 
	call  GetKey
 ;----------------------------------------------------;
 ; If Left or Right.                                  ;
 ;----------------------------------------------------;
	call  LorR 
	cmp   al,0
	je    n_r_l
	mov   [BarExit],0
	jmp   end_f
n_r_l:	
	mov   dx,file_max
	call  UorD
	cmp   al,0
	je    n_u_df 
	jmp   draw_f  
n_u_df: 
	cmp  [AsciiKey],escape_a
	je   end_escape_f
nextf1: 
	cmp   [AsciiKey],enter_a
	je    chk_y
	jmp   get_f
 ;----------------------------------------------------;
 ; LOAD.                                              ;
 ;----------------------------------------------------;
chk_y:	
	cmp   [WinY],1 ;4
	jne   chk_y2
	call  LoadAct
	jc    FddWriteError
	jmp   end_escape_f 
chk_y2:
 ;----------------------------------------------------;
 ; SAVE.                                              ;
 ;----------------------------------------------------;
	cmp   [WinY],2 
	jne   chk_y3
	call  WriteFileToDevice
	jc    FddWriteError
	jmp   end_escape_f 
chk_y3:
 ;----------------------------------------------------;
 ; SAVE AS.                                           ;
 ;----------------------------------------------------;
	cmp   [WinY],3
	jne   chk_y4
	call  Conver4TextFile
NewNameNeeded:
	call  Cursor_1
	mov   dl,5;6
	mov   dh,6 ;4
	call  GoToXYTexWin
	call  SaveAs
	mov   al,0x1f	
	call  [TextColor]
	mov   al,5 
	mov   ah,6 
	call  [SetCursorPos]
	mov   bx,100
	mov   dx,13
	call  [GetUserInput]
	jc    EscPressed
	mov   [WrittenFileName],edi
	mov   edx,[WrittenFileName]
	call  open
	jc    NoFileWithThatNameFound
	call  close
	call  GoToXY
	call  Cursor_0
	call  AskYesNo
	jc    NewNameNeeded
NoFileWithThatNameFound:
	mov   edx,[WrittenFileName]
	call  create
	jc    FddWriteError
	mov   dword[FileHandle3],ebx
KeepThisName:
	mov   edx,Buffer
	mov   ecx,[NumberOfBinFile]
	call  write
	jc    FddWriteError2
WhddOK:
	mov   ebx,dword[FileHandle3]
	call  close
	mov   [AdressAddOn],0			    ; craig new    t ok
 ;----------------------------------------------------;
 ; Any Write errors ?.                                ;
 ;----------------------------------------------------;
	call  SavedFileName
	call  ShowFileName
EscPressed:
	call  GoToXY
	call  Cursor_0
	jmp   end_escape_f
FddWriteError:
	mov   byte[ErrorCode],ah
	call  GoToXY
	call  Cursor_0
	call  FddError
	jmp   end_escape_f 
FddWriteError2:
	mov   ebx,dword[FileHandle3]
	call  close
	mov   [AdressAddOn],0			    ; craig new   2
	mov   byte[ErrorCode],ah
	call  GoToXY
	call  Cursor_0
	call  FddError
	jmp   end_escape_f
chk_y4:
 ;----------------------------------------------------;
 ; NEW.                                               ;
 ;----------------------------------------------------;
	cmp   [WinY],4
	jne   chk_y5
	call  New
	call  ShowFileName 
	call  info_win
	call  GoToXY
	jmp   end_escape_f
 chk_y5:  
 ;----------------------------------------------------;
 ; OS Shell.                                          ;
 ;----------------------------------------------------;
	cmp   [WinY],5
	jne   chk_y6
	call  PrintPage
	call  Win_Fasm
	call  Cursor_1
	mov   dl,10 
	mov   dh,6
	call  GoToXYTexWin
	mov   al,10 
	mov   ah,6
	call  [SetCursorPos]
	mov   al,0x7f
	call  [TextColor]
	mov   bx,100
	mov   dx,32 
	call  [GetCommandStored]
	jc    EscPressed 
	mov   [Params],edi
	call  Fasm
	call  UpDateFasmMessages
	call  [WaitForKeyPress]
	jmp   EscPressed 
chk_y6:
 ;----------------------------------------------------;
 ; Exit.                                              ;
 ;----------------------------------------------------;  
	cmp   [WinY],6
	je    ExitTex4u
	jmp   get_f			     
ExitTex4u:
       mov   [ExitEditor],1
       jmp   end_escape_f
end_escape_f:
	mov   [BarExit],1
end_f:
	pop   es
	popad
	ret

;====================================================;
; DrawEdit.                                          ;
;====================================================;
DrawEdit:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,4
	mov   ecx,6
loopeShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,23
	mov   ecx,18
	mov   al,Shadow1
rev_eShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_eShadow
	pop   ecx
	loop  loopeShadow
	mov   [WinY],1
	cld
 ;----------------------------------------------------;
 ; Draw File Window.                                  ;
 ;----------------------------------------------------;
draw_e: 
	mov   esi,WinEdit+3			      
	xor   ecx,ecx
	mov   cl,[WinEdit+1]
	xor   ebx,ebx
	mov   bx,0 ;12
loope2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,20 
	push  ecx
	xor   ecx,ecx
	mov   al,[WinEdit+2]
	mov   cl,[WinEdit]
loope1: 
	movsb
	stosb
	loop  loope1
	pop   ecx
	loop  loope2
 ;----------------------------------------------------;
 ; Mark Selection.                                    ;
 ;----------------------------------------------------;
	mov   bx,[WinY] 	    
	shl   bx,2 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4]
	add   edi,23
	xor   cx,cx
	mov   cl,[WinEdit]
	sub   cl,2
	mov   al,reverseMark
rev_e:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_e
get_e: 
	call  GetKey
 ;----------------------------------------------------;
 ; If Left or Right.                                  ;
 ;----------------------------------------------------;
	call  LorR 
	cmp   al,0
	je    n_r_le
	mov   [BarExit],0
	jmp   end_e
n_r_le:  
	mov   dx,Edit_max
	call  UorD
	cmp   al,0
	je    n_u_de
	jmp   draw_e  
n_u_de: 
	cmp  [AsciiKey],escape_a
	je   end_escape_e
	jmp  get_e	  
end_escape_e:
	mov   [BarExit],1
end_e:
	pop   es
	popad
	ret

;====================================================;
; DrawSearch.                                        ;
;====================================================;
DrawSearch:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,4 ;12
	mov   ecx,4
loopsShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,41
	mov   ecx,13
	mov   al,Shadow1
rev_sShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_sShadow
	pop   ecx
	loop  loopsShadow
	mov   [WinY],1
	cld
 ;----------------------------------------------------;
 ; Draw File Window.                                  ;
 ;----------------------------------------------------;
draw_s: 
	mov   esi,WinSearch+3				
	xor   ecx,ecx
	mov   cl,[WinSearch+1]
	xor   ebx,ebx
	mov   bx,0
loops2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,38 
	push  ecx
	xor   ecx,ecx
	mov   al,[WinSearch+2]
	mov   cl,[WinSearch]
loops1: 
	movsb
	stosb
	loop  loops1
	pop   ecx
	loop  loops2
 ;----------------------------------------------------;
 ; Mark Selection.                                    ;
 ;----------------------------------------------------;
	mov   bx,[WinY] 	    
	shl   bx,2 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4]
	add   edi,41
	xor   cx,cx
	mov   cl,[WinSearch]
	sub   cl,2
	mov   al,reverseMark
rev_s:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_s
get_s: 
	call  GetKey
 ;----------------------------------------------------;
 ; If Left or Right.                                  ;
 ;----------------------------------------------------;
	call  LorR 
	cmp   al,0
	je    n_r_ls
	mov   [BarExit],0
	jmp   end_s
n_r_ls:  
	mov   dx,Search_max
	call  UorD
	cmp   al,0
	je    n_u_ds 
	jmp   draw_s	       
n_u_ds:
	cmp  [AsciiKey],escape_a
	je   end_escape_s
	jmp  get_s	  
end_escape_s:
	mov   [BarExit],1
end_s:
	pop   es
	popad
	ret

;====================================================;
; DrawHelp.                                          ;
;====================================================;
DrawHelp:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,4
	mov   ecx,4
loophShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,63
	mov   ecx,17
	mov   al,Shadow1
rev_fhShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_fhShadow
	pop   ecx
	loop  loophShadow
	mov   [WinY],1
	cld
 ;----------------------------------------------------;
 ; Draw File Window.                                  ;
 ;----------------------------------------------------;
draw_h: 
	mov   esi,WinHelp+3			      
	xor   ecx,ecx
	mov   cl,[WinHelp+1]
	xor   ebx,ebx
	mov   bx,0 ;12
looph2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,60
	push  ecx
	xor   ecx,ecx
	mov   al,[WinHelp+2]
	mov   cl,[WinHelp]
looph1: 
	movsb
	stosb
	loop  looph1
	pop   ecx
	loop  looph2
 ;----------------------------------------------------;
 ; Mark Selection.                                    ;
 ;----------------------------------------------------;
	mov   bx,[WinY] 	    
	shl   bx,2 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4]
	add   edi,63
	xor   cx,cx
	mov   cl,[WinHelp]
	sub   cl,2
	mov   al,reverseMark
rev_h:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_h
get_h: 
	call  GetKey
 ;----------------------------------------------------;
 ; If Left or Right.                                  ;
 ;----------------------------------------------------;
	call  LorR 
	cmp   al,0
	je    n_r_lh
	mov   [BarExit],0
	jmp   end_h
n_r_lh:  
	mov   dx,Help_max
	call  UorD
	cmp   al,0
	je    n_u_dh 
	jmp   draw_h  
n_u_dh: 
	cmp  [AsciiKey],escape_a
	je   end_escape_h
	cmp   [AsciiKey],enter_a
	je    chk_yenter
	jmp   get_h 
 ;----------------------------------------------------;
 ; Help.                                              ;
 ;----------------------------------------------------;
chk_yenter:  
	cmp   [WinY],1 ;4
	jne   chk_NotCommends
	call  PrintPage
	call  Win_Commands
	jmp   end_escape_h
chk_NotCommends:
	cmp   [WinY],2 ;5
	jne   chk_NotAbout
	call  PrintPage
	call  Win_About
	jmp   end_escape_h
chk_NotAbout:
	jmp  get_h	  
end_escape_h:
	mov   [BarExit],1
end_h:
	pop   es
	popad
	ret

;====================================================;
; DrawScreen.                                        ;
;====================================================;
DrawScreen:
	pushad
	call  TopGreenBar
	popad
DrawScreenHalf:
	pushad
	call  TopBorder
	call  BottomGreenBar
	call  info_win
	popad
	ret

;====================================================;
; LorR.                    ;Right or Left Key <- ->  ;
;====================================================;
LorR:			  
	mov   al,0		   ;Set a Boolean AL=0
	cmp   [ScanKey],left
	jne   next1
	cmp   [WinX],1
	ja    x_min
	mov   [WinX],win_max+1
x_min: 
	dec   [WinX]
	mov   al,1
next1:	
	cmp   [ScanKey],right
	jne   next2
	cmp   [WinX],win_max
	jb    x_max
	mov   [WinX],0
x_max: 
	inc   [WinX]
	mov   al,1		   ;Right or Left is True
next2:	
	ret

;====================================================;
; UorD.                            ;Up or Down Key   ;
;====================================================;
UorD:
	mov   al,0		   ;Set a Boolean AL=0
	cmp   [ScanKey],up
	jne   nextv1
	cmp   [WinY],1 ;4 ;1
	ja    y_min
	mov   [WinY],dx
	inc   [WinY]
y_min:	
	dec   [WinY]
	mov   al,1		   ;Up or Down is True
nextv1: 
	cmp   [ScanKey],down
	jne   nextv2
	cmp   [WinY],dx
	jb    y_max
	mov   [WinY],0 ;3
y_max:	
	inc   [WinY]
	mov   al,1		   ;Up or Down is True
nextv2: 
	ret

;====================================================;
; DrawScreen.                                        ;
;====================================================;
WinHeadSetColorreversed:
	pushad
	push  es
	mov   ax,8h
	mov   es,ax
	mov   edi,0xb8000
	mov   esi,edi
	cmp   [WinX],1
	jne   N1
	add   edi,7
	xor   ecx,ecx 
	mov   cx,6		   ;[File] be reversed
Loop1:	  
	cmp   byte [es:edi],0x8f ;0x7c
	je    @f
	mov   al,reverse1
	mov   [es:edi],al
	jmp   JustReverse1
@@:
	mov   al,0x2f
	mov   [es:edi],al
JustReverse1:
	add   edi,2
	loop  Loop1
N1: 
	cmp   [WinX],2
	jne   N2
	add   edi,25
	mov   cx,6		   ;[Edit] be reversed
Loop2:	  
	cmp   byte [es:edi],0x8f
	je    @f
	mov   al,reverse1
	mov   [es:edi],al
	jmp   JustReverse2
@@:
	mov   al,0x2f
	mov   [es:edi],al
JustReverse2:
	add   edi,2
	loop  Loop2
N2:
	cmp   [WinX],3
	jne   N3
	add   edi,43
	mov   cx,8		   ;[Search] be reversed
Loop3:	  
	cmp   byte [es:edi],0x8f
	je    @f
	mov   al,reverse1
	mov   [es:edi],al
	jmp   JustReverse3
@@:
	mov   al,0x2f
	mov   [es:edi],al
JustReverse3:
	add   edi,2
	loop  Loop3
N3:
	cmp   [WinX],4
	jne   N4
	add   edi,65
	mov   cx,6		   ;[Help] be reversed
Loop4:	  
	cmp   byte [es:edi],0x8f
	je    @f
	mov   al,reverse1
	mov   [es:edi],al
	jmp   JustReverse4
@@:
	mov   al,0x2f
	mov   [es:edi],al
JustReverse4:
	add   edi,2
	loop  Loop4
N4:
	pop   es
	popad
	ret


;====================================================;
; SaveAs.                                            ;
;====================================================;
SaveAs:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,6
loopsaShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,7
	mov   ecx,32
	mov   al,Shadow1
rev_saShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_saShadow
	pop   ecx
	loop  loopsaShadow
	cld
 ;-------------------;
 ; Draw File Window. ;
 ;-------------------; 
	mov   esi,Save_AsA+1
DrawSaveAs2:
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,6
loopSA2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,4 
	push  ecx
	xor   ecx,ecx
	mov   al,color2a ;[window_colors] 
	mov   cl,[Save_AsA]
loopSA1: 
	movsb
	stosb
	loop  loopSA1
	pop   ecx
	loop  loopSA2
	xor   ebx,ebx
	mov   bx,24
	mov   edi,0xb8000
	add   edi,[Ytable+ebx] 

	add   edi,9 
	mov   ecx,28
	mov   al,1Fh

loopSA1ABC: 
	stosb
	inc   edi
	loop  loopSA1ABC

	pop   ds
	pop   es
	popad
	ret

YesNoVar db 0
;====================================================;
;AskYesNo.                                           ;
;====================================================;
AskYesNo :
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	cld
 ;-------------------;
 ; Draw File Window. ;
 ;-------------------; 
DrawSaveAs1YesNo:
	mov   esi,Save_AsYesNo+1
DrawSaveAs2YesNo:
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,6
loopSA2YesNo: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,4 
	push  ecx
	xor   ecx,ecx
	mov   al,[window_colors]
	mov   cl,[Save_AsYesNo]
loopSA1YesNo: 
	movsb
	stosb
	loop  loopSA1YesNo
	pop   ecx
	loop  loopSA2YesNo
	mov   [YesNoVar],1
	jmp   AnswerYes
AnswerYesGetKey:
	call  GetKey
	cmp   [ScanKey],left
	jne   @f
	cmp   [YesNoVar],1
	je    AnswerYesGetKey
	mov   [YesNoVar],1
	jmp   AnswerYes
@@:
	cmp   [ScanKey],right
	jne   @f
	cmp   [YesNoVar],0
	je    AnswerYesGetKey
	mov   [YesNoVar],0
	jmp   AnswerNo
@@:
	cmp   [ScanKey],enter_s
	jne   AnswerYesGetKey
	cmp   [YesNoVar],0
	je    AnswerNoExit
	jmp   AnswerYesExit
AnswerYes:
	xor   ebx,ebx
	mov   bx,28
	mov   edi,0xb8000
	add   edi,[Ytable+ebx] 
	mov   edx,edi
	add   edi,23 ;cc
	mov   ecx,5
	mov   al,1Fh
loopAnswerYes1: 
	stosb
	inc   edi
	loop  loopAnswerYes1
	mov   edi,edx
	add   edi,41 ;cc
	mov   ecx,4
	mov   al,3Fh
loopAnswerYes2: 
	stosb
	inc   edi
	loop  loopAnswerYes2
	jmp   AnswerYesGetKey
AnswerNo:
	xor   ebx,ebx
	mov   bx,28
	mov   edi,0xb8000
	add   edi,[Ytable+ebx] 
	mov   edx,edi
	add   edi,41 
	mov   ecx,4
	mov   al,1Fh

loopAnswerNo1: 
	stosb
	inc   edi
	loop  loopAnswerNo1
	mov   edi,edx
	add   edi,23 
	mov   ecx,5
	mov   al,3Fh
loopAnswerNo2: 
	stosb
	inc   edi
	loop  loopAnswerNo2
	jmp   AnswerYesGetKey
AnswerYesExit:
	pop   ds
	pop   es
	popad
	clc
	ret

AnswerNoExit:
	pop   ds
	pop   es
	popad
	stc 
	ret



;====================================================;
; FddError.                                          ;
;====================================================;
FddError:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,6 
loopfdeShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,7
	mov   ecx,32
	mov   al,Shadow1
rev_fdeShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_fdeShadow
	pop   ecx
	loop  loopfdeShadow
	cld
 ;-------------------;
 ; Draw File Window. ;
 ;-------------------;
DrawFddError: 
	mov   esi,Fdd_Error+1				
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,6 
loopFE2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,4 
	push  ecx
	xor   ecx,ecx
	mov   al,error1      
	mov   cl,[Fdd_Error]
loopFE1: 
	movsb
	stosb
	loop  loopFE1
	pop   ecx
	loop  loopFE2
	call  Beep
	call  [WaitForKeyPress] 
	pop   ds
	pop   es
	popad
	ret


;====================================================;
; Win_Fasm.                                          ;
;====================================================;
Win_Fasm:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw Fasm Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,19
loopFShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,9 
	mov   ecx,74 
	mov   al,Shadow1
rev_FShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_FShadow
	pop   ecx
	loop  loopFShadow
	cld
 ;-------------------;
 ; Draw Fasm Window. ;
 ;-------------------;
DrawWinFasm:
	mov   esi,WinFasm+1
	xor   ebx,ebx
	mov   bx,8
	mov   ecx,19
loopWA2F:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,6 ;26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinFasm]
loopWA1F:
	movsb
	stosb
	loop  loopWA1F
	pop   ecx
	loop  loopWA2F
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; UpDateFasmMessages.                                ;
;====================================================;
UpDateFasmMessages:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
	mov   [UpDateFasmLine],1120
FasmUpDate:
	mov   esi,FasmScreenBuffer
	xor   ebx,ebx
	mov   bx,13
	mov   ecx,14
loopFasmUpDate2:
	mov   edi,0xb8000
	add   edi,[UpDateFasmLine]
	add   edi,10
	add   [UpDateFasmLine],160
	push  ecx
	xor   ecx,ecx
	mov   cl,70 
loopFasmUpDate1:
	movsw
	loop  loopFasmUpDate1
	pop   ecx
	loop  loopFasmUpDate2
	pop   ds
	pop   es
	popad
	ret


;====================================================;
; Win_About.                                         ;
;====================================================;
Win_About:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,19
loopabShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_abShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_abShadow
	pop   ecx
	loop  loopabShadow
	cld
 ;-------------------;
 ; Draw About Window.;
 ;-------------------;
DrawWinAbout: 
	mov   esi,WinAbout+1			       
	xor   ebx,ebx
	mov   bx,8 ;52
	mov   ecx,19
loopWA2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinAbout]
loopWA1: 
	movsb
	stosb
	loop  loopWA1
	pop   ecx
	loop  loopWA2
	call  [WaitForKeyPress]
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Win_Commands.                                      ;
;====================================================;
Win_Commands:
       pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   ax,18h
	mov   ds,ax
 ;----------------------------------------------------;
 ; Draw File Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,16
	mov   ecx,17
loopcShadow:
	push  ecx
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,29
	mov   ecx,54
	mov   al,Shadow1
rev_cShadow:
	mov   [es:edi],al
	add   edi,2
	loop  rev_cShadow
	pop   ecx
	loop  loopcShadow
	cld
 ;----------------------;
 ; Draw Commands Window.;
 ;----------------------;
DrawWinCommands: 
	mov   esi,WinCommands+1 			  
	xor   ebx,ebx
	mov   bx,12
	mov   ecx,17
loopWC2: 
	mov   edi,0xb8000
	add   edi,[Ytable+ebx+4] 
	add   bx,4 
	add   edi,26 
	push  ecx
	xor   ecx,ecx
	mov   al,0x78
	mov   cl,[WinCommands]
loopWC1: 
	movsb
	stosb
	loop  loopWC1
	pop   ecx
	loop  loopWC2
	call  [WaitForKeyPress]
	pop   ds
	pop   es
	popad
	ret
 ;----------------------------------------------------;
 ; Data                                               ;
 ;----------------------------------------------------;

 UpDateFasmLine dd  0

