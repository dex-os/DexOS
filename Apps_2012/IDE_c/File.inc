DefaultListNumber	    = 1024
NunmberOfLetters	    = 14
SetFileNumberCountMeaning   = 10 
SetFileNumberCountMeaning_1 = SetFileNumberCountMeaning -1
MarkedBoxSize		    = 39 
ListXoffSet		    = 38 
ListXoffSet_1		    = ListXoffSet -1
MenuYOffSet		    = 7 
MenuYOffSet_1		    = MenuYOffSet -1

;====================================================;
; NoModuleLoaded.                                    ;
;====================================================;
NoModuleLoaded:
	stc
	ret
;====================================================;
; ShowFileName.                                      ;
;====================================================;
ShowFileName:
	pushad	   
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   edi,dword[OutPutMonitor]
	add   edi,[Ytable+24*4]
	add   edi,2    
	mov   esi,FileName
	mov   al,0x2f
	xor   ecx,ecx
	mov   cl,12
llla:	
	movsb
	stosb
	loop  llla
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; ShowName.                                          ;
;====================================================;
ShowName: 
	pushad	   
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   edi,dword[OutPutMonitor]
	add   edi,[Ytable+18*4]
	add   edi,ListXoffSet-2 
	mov   ecx,80 ;40
	mov   al,77h ;33h
	rep   stosb
	mov   edi,dword[OutPutMonitor]
	add   edi,[Ytable+18*4]
	add   edi,ListXoffSet-2 
	mov   esi,ThisDir
	inc   esi
	mov   al,color2a ;[box_colors1]
	mov   cl,byte[ThisDir]
	xor   ch,ch
lll:	
	movsb
	stosb
	loop  lll
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; CurrantDir.                                        ;
;====================================================;
CurrantDir:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax

	mov   al,[DriveVar]
	mov   [ThisDir+1],al
	mov   byte[ThisDir+2],':'
	mov   byte[ThisDir+3],'\'
 ;----------------------------------------------------;
 ; Get Current Default Dir                            ;
 ;----------------------------------------------------;                                                        ;
	mov   esi,ThisDir
	add   esi,4
	mov   eax,15
	call  [Fatmod]					; Get_Current_Directory
	mov   al,4
	mov   ecx,64
	mov   edi,ThisDir
	add   edi,4
cpr_0:	cmp   byte [es:edi],0
	je    FoundLetsGo
	inc   edi
	inc   al
	loop  cpr_0

FoundLetsGo:
	cmp   al,4
	je    @f
@@:
	mov   byte[ThisDir],al
	call  ShowName
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; info_win.                                          ;
;====================================================;
info_win:
	pushad
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	call  ConvertXtoDec
	mov   esi,str_y
	mov   ax,[CursorY]
	call  ConvertYtoDec
	mov   esi,str_ttl
	mov   ax,[TotalLine]
	call  ConvertYtoDec
 ;----------------------------------------------------;
 ; Print the total, line, col.                        ;
 ;----------------------------------------------------;
	cld
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+24*4]
	add   edi,42*2
	mov   cx,38
	mov   esi,win_info
	add   esi,42
	mov   al,0x2f
loop_i: 
	movsb
	stosb
	loop  loop_i
 ;----------------------------------------------------;
 ; Put the black lines in                             ;
 ;----------------------------------------------------;
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+24*4]
	add   edi,44*2+1
	mov   al,0x2f 
	mov   [es:edi],al
	add   edi,14*2
	mov   al,0x2f 
	mov   [es:edi],al
	pop   ds
	pop   es
	popad
	ret


;====================================================;
; InfoWinBackGround.                                 ;
;====================================================;
InfoWinBackGround:
	pushad
	cld
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+24*4]
	mov   cx,80
	mov   esi,win_info
	mov   al,0x2f
loop_ib:
	movsb
	stosb
	loop  loop_ib
	popad
	ret


;====================================================;
; LoadSave.                                          ;
;====================================================;
LoadSave:
	pushad 
	push  es
	push  ds
 ;----------------------------------------------------;
 ; Draw Fasm Shadow.                                  ;
 ;----------------------------------------------------;
	xor   ebx,ebx
	mov   bx,4		     
	shl   ebx,2		     
	mov   ecx,18
loopFShadowLoadSave:
	push  ecx
	mov   edi,0xb8000
	add   edi,dword[Ytable+ebx]  
	add   bx,4 
	add   edi,19 
	mov   ecx,54 
	mov   al,Shadow1
rev_FShadowLoadSave:
	mov   [es:edi],al
	add   edi,2
	loop  rev_FShadowLoadSave
	pop   ecx
	loop  loopFShadowLoadSave
	cld
 ;-------------------;
 ; Draw Fasm Window. ;
 ;-------------------;
DrawWinFasmLoadSave:
	mov   esi,WinLoadSave+1
	xor   ebx,ebx
	mov   bx,3
	shl   ebx,2
	mov   ecx,18
loopWA2FLoadSave:
	mov   edi,0xb8000
	add   edi,[Ytable+ebx] 
	add   bx,4 
	add   edi,16 
	push  ecx
	xor   ecx,ecx
	mov   al,color2a
	mov   cl,[WinLoadSave]
loopWA1FLoadSave:
	movsb
	stosb
	loop  loopWA1FLoadSave
	pop   ecx
	loop  loopWA2FLoadSave	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CL = color    
; AL = Box start x
; AH = Box start y
; DL = Box x
; DH = Box y 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ah,MenuYOffSet
	mov   al,ListXoffSet-2
	mov   dl,MarkedBoxSize
	mov   dh,SetFileNumberCountMeaning
	mov   cl,[box_colors]
	call  DrawAbox	

	mov   ah,5 
	mov   al,ListXoffSet-2
	mov   dl,MarkedBoxSize
	mov   dh,1 
	mov   cl,[box_colors]
	call  DrawAbox	  
	call  CurrantDir
	call  ShowName
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; LoadAct.                                           ;
;====================================================;
LoadAct:  
	pushad 
	push  es
	push  ds
	mov   ax,8h
	mov   es,ax
	mov   [LoadWinY],MenuYOffSet 
	call  GetList
	jc    ReadError
@@:
	mov   dword[ListDownCount],0
	mov   dword[progMenu1],esi
	mov   dword[FddFilesForMenu],edi
	mov   dword[FileNumberCount],ebx
	mov   dword[FileNumberCount2],ebx
	cmp   ebx,SetFileNumberCountMeaning_1
	jbe   ListNumberOK
	mov   dword[FileNumberCount2],SetFileNumberCountMeaning
ListNumberOK:
	mov   ax,8h
	mov   es,ax
	call  PrintPage
	call  LoadSave
TryAgain:
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+MenuYOffSet*4]
	add   edi,ListXoffSet
	mov   esi,dword[FddFilesForMenu]
	mov   ax,8h
	mov   es,ax
	mov   al,[box_colors]
	mov   ecx,dword[FileNumberCount2] 
	xor   ebx,ebx
	mov   bl,NunmberOfLetters  
dw_ld11: 
	push  ecx
	mov   cx,bx
f_ld11:  
	movsb
	stosb
	loop  f_ld11
	add   edi,160
	sub   edi,ebx
	sub   edi,ebx
	add   esi,1
	pop   ecx
	loop  dw_ld11 
 ;----------------------------------------------------;
 ; Mark Selection Load.                               ;
 ;----------------------------------------------------;
MarkLoadFile:
	mov   bx,[LoadWinY]		
	shl   bx,2 
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+ebx]
	add   edi,ListXoffSet_1    
	mov   ecx,MarkedBoxSize  
	mov   al,[box_selection_colors] 
rev_l:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_l
	call  UpdateTextBoxFile
LoadFileGetKey:
	call  GetKey
	cmp   [ScanKey],up
	jne   NextLoad
	cmp   [LoadWinY],MenuYOffSet
	jbe   LoadFileGetKeyTestUp
 ;----------------------------------------------------;
 ; Clear the marked part.                             ;
 ;----------------------------------------------------;
	mov   bx,[LoadWinY]		
	shl   bx,2 
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+ebx]
	add   edi,ListXoffSet_1  
	mov   ecx,MarkedBoxSize  
	mov   al,[box_colors] 
StopFlickering1:	
	mov   [es:edi],al
	add   edi,2
	loop  StopFlickering1
	dec   [LoadWinY]
	jmp   MarkLoadFile
 ;----------------------------------------------------;
 ; Test for redrawing list.                           ;
 ;----------------------------------------------------;
LoadFileGetKeyTestUp:
	cmp   dword[FileNumberCount2],SetFileNumberCountMeaning_1 
	jbe   LoadFileGetKey
	cmp   [ListDownCount],1
	jb    LoadFileGetKey
	dec   [ListDownCount]
	call  UpdateListMenu
	jmp   LoadFileGetKey
NextLoad:
	cmp   [ScanKey],down
	jne   NextLoad1
	mov   eax,dword[FileNumberCount2]
	add   eax,MenuYOffSet_1 		    
	cmp   [LoadWinY],ax
	jae   LoadFileGetKeyTest
	mov   bx,[LoadWinY]		
	shl   bx,2 
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+ebx]
	add   edi,ListXoffSet_1 		  
	mov   ecx,MarkedBoxSize 
	mov   al,[box_colors] 
StopFlickering2:	
	mov   [es:edi],al
	add   edi,2
	loop  StopFlickering2
	inc   [LoadWinY]
	jmp   MarkLoadFile 
LoadFileGetKeyTest:
	cmp   dword[FileNumberCount2],SetFileNumberCountMeaning_1
	jbe   LoadFileGetKey
	mov   eax,[ListDownCount]
	add   eax,SetFileNumberCountMeaning  
	cmp   eax,dword[FileNumberCount]
	jae    LoadFileGetKey
	inc   [ListDownCount]
	call  UpdateListMenu
	jmp   LoadFileGetKey
NextLoad1:
	cmp   [ScanKey],escape
	je    LoadExit
NextLoad2:
	cmp   [ScanKey],enter_s
	jne   NextLoad3
	call  LoadFileFromFloppy
	jc    ReadError
	cmp   byte[ChangeOK],1				
	jne   LoadExit
	mov   byte[ChangeOK],0
	mov   ax,8h
	mov   es,ax
	mov   [LoadWinY],MenuYOffSet 
	call  GetList
	jc    ReadError
@@:
	mov   dword[ListDownCount],0
	mov   dword[progMenu1],esi
	mov   dword[FddFilesForMenu],edi
	mov   dword[FileNumberCount],ebx
	mov   dword[FileNumberCount2],ebx
	cmp   ebx,SetFileNumberCountMeaning_1
	jbe   ListNumberOK2
	mov   dword[FileNumberCount2],SetFileNumberCountMeaning
ListNumberOK2:
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; CL = color    
; AL = Box start x
; AH = Box start y
; DL = Box x
; DH = Box y 
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	mov   ah,MenuYOffSet
	mov   al,ListXoffSet-2
	mov   dl,MarkedBoxSize
	mov   dh,SetFileNumberCountMeaning
	mov   cl,11h 
	call  DrawAbox	

	mov   ah,5 
	mov   al,ListXoffSet-2
	mov   dl,MarkedBoxSize
	mov   dh,1 
	mov   cl,11h 
	call  DrawAbox	  
	call  CurrantDir
	;call  ShowName
	jmp   TryAgain
NextLoad3:
	jmp   LoadFileGetKey
LoadExit:
	pop   ds
	pop   es
	popad
	clc
	ret

ReadError:
	mov   byte[ErrorCode],al
	pop   ds
	pop   es
	popad
	mov   al,byte[ErrorCode]
	stc 
	ret

ListCountTEST dd 0
;====================================================;
;  UpdateTextBoxFile                                 ;
;====================================================;
UpdateTextBoxFile:
	pushad
	push  es
	push  ds
	xor   eax,eax
	mov   ax,[LoadWinY]
	sub   eax,MenuYOffSet		
	mov   ebx,eax
	add   ebx,[ListDownCount]
	add   eax,[ListDownCount]
	shl   eax,4
	sub   eax,ebx
	mov   esi,eax
	add   esi,[FddFilesForMenu]
	mov   al,byte[esi]
	cmp   al,9h
	je    UpdateTextBoxFileExit					 
	cmp   al,18h
	je    UpdateTextBoxFileExit
	add   esi,2
	mov   bx,5	   
	shl   bx,2 
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+ebx]
	add   edi,ListXoffSet 
	mov   ecx,12 
UpDateTextBox:	
	movsb
	mov   al,[box_colors]
	stosb
	loop  UpDateTextBox
UpdateTextBoxFileExit:
	pop   ds
	pop   es
	popad
	ret

;====================================================;
;  Redraw the list.                                  ;
;====================================================;
UpdateListMenu:
	pushad
	push  es
	push  ds
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+MenuYOffSet*4]	     
	add   edi,ListXoffSet 
	mov   esi,dword[FddFilesForMenu]
	mov   ebx,[ListDownCount]
	mov   eax,[ListDownCount]
	shl   eax,4
	sub   eax,ebx
	add   esi,eax
	mov   ax,8h
	mov   es,ax
	mov   al,[box_colors] 
	mov   ecx,dword[FileNumberCount2] 
	xor   ebx,ebx
	mov   bl,NunmberOfLetters  
dw_ld11UD: 
	push  ecx
	mov   cx,bx

f_ld11UD:  
	movsb
	stosb
	loop  f_ld11UD
	add   edi,160
	sub   edi,ebx
	sub   edi,ebx
	add   esi,1
	pop   ecx
	loop  dw_ld11UD 
 ;----------------------------------------------------;
 ; Mark Selection Load.                               ;
 ;----------------------------------------------------;
MarkLoadFileUD:
	mov   bx,[LoadWinY]		
	shl   bx,2 
	mov   edi,[OutPutMonitor]
	add   edi,[Ytable+ebx]
	add   edi,ListXoffSet_1 
	mov   ecx,MarkedBoxSize  
	mov   al,[box_selection_colors] 
rev_lUD:	
	mov   [es:edi],al
	add   edi,2
	loop  rev_lUD
	call  UpdateTextBoxFile
	pop   ds
	pop   es
	popad
	clc
	ret

;====================================================;
; LoadFileFromFloppy.                                ;
;====================================================;
LoadFileFromFloppy:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   edi,Buffer
	mov   eax,0
	mov   ecx,MaxFileSize
	rep   stosb
	xor   eax,eax
	mov   ax,[LoadWinY]
	sub   eax,MenuYOffSet		
	mov   ebx,eax
	add   ebx,[ListDownCount]
	add   eax,[ListDownCount]
	shl   eax,4
	sub   eax,ebx
	mov   esi,eax
	add   esi,[FddFilesForMenu]
	mov   al,byte[esi]
	cmp   al,7h
	je    WeHaveAfile
	call  ChangeDirLoad
	jc    FddLoadError
	jmp   FddLoadOk
WeHaveAfile:
	add   esi,2
	mov   edx,esi
	mov   [FileAddress],esi
	call  open
	jc    FddLoadError
	mov   dword[FileHandle2],ebx
	mov   edx,Buffer
	mov   ecx,MaxFileSize
	call  read
	jc    FddLoadError2
	xor   ecx,ecx
	mov   ebx,ecx
	call  ConvertFileName
	call  ShowFileName
	call  ClearBuffer
	mov   ebx,Buffer  
	mov   [BufferAddOn1],ebx 
	mov   [ShowText],1
	mov   [LineCount],1 
	mov   [CursorX],1
	mov   [CursorY],1
	mov   [TopOfScreen],1
	mov   edi,PageUpPointerAray1 
	mov   [PageUpPointer],edi 
	call  NumberOfLines
	mov   edi,PageUpPointerAray1 
	mov   [PageUpPointer],edi 
	mov   [EndOfFile],0
	mov   [ColStart],0
	call  PrintPage
	call  info_win
	mov   [PosX],0 
	mov   [PosY],2
	call  GoToXY
FddLoadOk:
	mov   ebx,dword[FileHandle2]
	call  close
	mov   [AdressAddOn],0			    ; craig new
	pop   ds
	pop   es
	popad
	clc  
	ret

ChangeOK db 0
;=======================================================  ;
; ChangeDirLoad.                                          ;
;=======================================================  ;
ChangeDirLoad:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ax,18h					  ;     
	mov	es,ax					  ;
	mov	ds,ax					  ;
	mov	byte[ChangeOK],0			  ;
	add	esi,2					  ;
	mov	edx,esi 				  ;
	mov	eax,7					  ;
	call	[Fatmod]				  ; Set_Current_Directory
	jc	FddLoadError				  ;
	mov	byte[ChangeOK],1			  ;
  ChangeDirLoadOk:					  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	ret						  ;

FddLoadError:
	mov   byte[ErrorCode],al
	pop   ds
	pop   es
	popad
	mov   al,byte[ErrorCode]
	stc  
	ret

FddLoadError2:
	mov   byte[ErrorCode],al
	mov   ebx,dword[FileHandle2]
	call  close
	mov   [AdressAddOn],0			    ; craig new
	pop   ds
	pop   es
	popad
	mov   al,byte[ErrorCode]
	stc  
	ret


SetBuffer:
	pushad
	push  es
	mov   ax,18h
	mov   es,ax
	mov   cx,38
@@:   
	push  cx
	mov   al,' '
	mov   cx,78
	mov   edi,Buffer
	cld
	rep   stosb
	mov   al,13
	stosb
	stosb
	pop   cx
	loop  @b

	mov   al,0
	mov   cx,80*10
	rep   stosb
	pop   es
	popad
	ret

NoFileName:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   esi,FileName1
	mov   edi,FileName
	mov   cx,12
	rep   movsb
	pop   ds
	pop   es
	popad
	ret

SavedFileName:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   esi,[WrittenFileName]
	mov   edi,FileName
	mov   cx,12
	rep   movsb
	pop   ds
	pop   es
	popad
	ret

ConvertFileName:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   es,ax
	mov   ds,ax
	mov   edi,FileName
	mov   cx,12
	mov   al,' '
	rep   stosb
	mov   esi,[FileAddress]
	mov   edi,FileName
	mov   cx,12
	rep   movsb
ConvertFileNameEnd:
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; Conver4TextFile.                                   ;
;====================================================;
Conver4TextFile:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	mov   [NumberOfBinFile],0
	mov   edx,[NumberOfLinesInFile]
	mov   [C4tfNumbeoFLoops],edx
	mov   [FileBufferAddOn],0
	mov   [FileBufferAddOn1],255
	mov   edi,Buffer
	mov   [EdiAddress],edi
Conver4TextFileLoop1:
	mov   esi,PageUpPointerAray1 
	add   esi,[FileBufferAddOn1]
	mov   ecx,256  
	std
Conver4TextFileLoop2:
	lodsb
	cmp   al,' '
	jne   @f
	loop  Conver4TextFileLoop2
@@:	
	cld
	mov   [EOFlNumber],ecx
	mov   esi,PageUpPointerAray1 
	add   esi,[FileBufferAddOn]
	mov   edi,[EdiAddress]
	add   [NumberOfBinFile],ecx
	add   [NumberOfBinFile],2
	rep   movsb
	mov   ax,0x0a0d  
	stosw
	mov   [EdiAddress],edi
	add   [FileBufferAddOn],256 
	add   [FileBufferAddOn1],256
	dec   [C4tfNumbeoFLoops]
	cmp   [C4tfNumbeoFLoops],0
	jne   Conver4TextFileLoop1
	sub   [NumberOfBinFile],2
	sub   edi,2
	mov   al,00
	mov   cx,550
	rep   stosb
	pop   ds
	pop   es
	popad
	ret

;====================================================;
; WriteFileToDevice.                                 ;
;====================================================;
WriteFileToDevice:
	pushad
	push  es
	push  ds
	mov   ax,18h
	mov   ds,ax
	mov   es,ax
	call  Conver4TextFile
	mov   edx,FileName
	call  create
	jc    WriteFileToDeviceError
	mov   dword[FileHandle3],ebx
	mov   ecx,dword[NumberOfBinFile]
	; ecx should = file size in bytes
	mov   ebx,dword[FileHandle3]
	mov   edx,Buffer
	call  write
	jc    WriteFileToDeviceError2
	mov   ebx,dword[FileHandle3]
	call  close
WriteFileToDeviceOK:
	pop   ds
	pop   es
	popad
	clc
	ret	

WriteFileToDeviceError:
	mov   byte[ErrorCode],al
	pop   ds
	pop   es
	popad
	mov   al,byte[ErrorCode]
	stc
	ret	

WriteFileToDeviceError2:
	mov   byte[ErrorCode],al
	mov   ebx,dword[FileHandle3]
	call  close
	pop   ds
	pop   es
	popad
	mov   al,byte[ErrorCode]
	stc
	ret	

FileHandle3 dd 0


IdeFileSizeCount dd 0	
;=======================================================  ;
;  GetFileSizeInBytes                                     ;
;=======================================================  ;
GetFileSizeInBytes:					  ;
	pushad						  ;
	push  es					  ;
	push  ds					  ;
	mov   ax,18h					  ;
	mov   ds,ax					  ;
	mov   es,ax					  ;
	mov   dword[IdeFileSizeCount],0 		  ;
	mov   esi,Buffer				  ;
	mov   ecx,1024*1024*2				  ;
GetFileSizeInBytesMainLoop:				  ;
	lodsb						  ;
	cmp   al,1Ah					  ; is it end of file?
	je    GetFileSizeInBytesExit			  ;
	cmp   al,0					  ; is it end of file?
	je    GetFileSizeInBytesExit			  ;
	inc   dword[IdeFileSizeCount]			  ;
	loop  GetFileSizeInBytesMainLoop		  ;
GetFileSizeInBytesExit: 				  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	mov	ecx,dword[IdeFileSizeCount]		  ;
	ret						  ;

;=======================================================  ;
;  clear buffer.                                          ;
;=======================================================  ;
GetList:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	mov	dword[ListBufferAddOn],0		  ;
	mov	dword[File_Count],0			  ;
;=======================================================  ;
;  clear buffers.                                         ;
;=======================================================  ;
	mov	edi,ListBuffer				  ;
	mov	ecx,15*DefaultListNumber		  ;
	mov	al,' '					  ;
	rep	stosb					  ;
	mov	al,' '					  ;
	mov	ecx,64					  ;
	mov	edi,TempBuff1				  ;
	rep	stosb					  ;
;=======================================================  ;
; add a slash.                                            ;
;=======================================================  ;
	mov	edi,TempBuff1				  ;
	mov	al,'\'					  ;
	stosb						  ;
;=======================================================  ;
;  get current directory.                                 ;
;=======================================================  ;
	mov	esi,TempBuff1+1 			  ;
	mov	eax,15					  ;
	call	[Fatmod]				  ; Get_Current_Directory
	jc	GetListError				  ; jump if carry flag is set
;=======================================================  ;
;  test for cli arguements.                               ;
;=======================================================  ;
	cmp	[ChangeDirTest],0			  ;
	je	NoArguements				  ;
;=======================================================  ;
;  change to cli dir.                                     ;
;=======================================================  ;
	call	ChangeDir				  ;
  NoArguements: 					  ;
;=======================================================  ;
;  get current drive.                                     ;
;=======================================================  ;
	call	[GetBootDrive]
	inc	bl					  ;
	mov	byte [drive],bl 			  ;
	add	bl,40h					  ;
	mov	byte[DriveNew],bl			  ;
	mov	byte[Drive2],bl 			  ;
;=======================================================  ;
;  redirect DTA to new buffer                             ;
;=======================================================  ;
	mov	edx,DTAbuffer				  ;
	mov	eax,3					  ;
	call	[Fatmod]				  ; SetDTA_Address
	mov	edx,FileNames				  ;
	mov	ecx,3dh 				  ; = do not show hidden files; show all files = 3fh
	mov	eax,16					  ;
	call	[Fatmod]				  ; Find_First
	jc	GetListError2				  ; jump if carry flag is set
	mov	esi,DTAbuffer +15h			  ;
	mov	al,byte[esi]				  ;
	test	al,10					  ;
	jz	@f ;VolumeIsNotFirst                      ;
	jmp	LoopCycle				  ;
  @@:							  ;
	test	al,10h					  ;
	jnz	@f					  ;
	jmp	LoopCycle				  ;
  @@:							  ;
;=======================================================  ;
;  dir listing starts here.                               ;
;=======================================================  ;                                  
	mov	edi,ListBuffer				  ;
	add	edi,[ListBufferAddOn]			  ;
	mov	esi,fileName				  ;
	mov	al,byte[esi]				  ;
	cmp	al,'.'					  ;
	jne	NotAdot0				  ;
	mov	al,byte[esi+1]				  ;
	cmp	al,'.'					  ;
	jne	 LoopCycle				  ;
	mov	al,18h					  ;
	stosb						  ;
	jmp	YesAdot0				  ;
  NotAdot0:						  ;
	mov	al,9h					  ;
	stosb						  ;
  YesAdot0:						  ;
	mov	al,' '					  ;
	stosb						  ;
	mov	esi,fileName				  ;
	mov	ecx,13					  ;
	rep	movsb					  ;
	add	[ListBufferAddOn],15			  ;
	mov	edi,fileName				  ;
	mov	al,0					  ;
	mov	ecx,14					  ;
	rep	stosb					  ;
	inc	dword[File_Count]			  ;
	cmp	dword[File_Count],DefaultListNumber	  ;
	jae	GetListError2				  ;
	jmp	LoopCycle				  ;
;=======================================================  ;
;  Main loop.                                             ;
;=======================================================  ;
  LoopCycle:						  ;
	mov	edi,fileName				  ;
	mov	al,0					  ;
	mov	ecx,14					  ;
	rep	stosb					  ;     
	mov	edx,FileNames				  ; DS:DX points to file name
	mov	eax,17					  ;
	call	[Fatmod]				  ; Find_Next
	jc	DirListFinish				  ; try files, if carry flag is set
	mov	esi,DTAbuffer +15h			  ;
	mov	al,byte[esi]				  ;
	cmp	al,10h					  ;
	je	@f					  ;
	jmp	LoopCycle				  ;
  @@:							  ;
;=======================================================  ;
;  test if its a dir.                                     ;
;=======================================================  ;
	mov	edi,ListBuffer				  ;
	add	edi,[ListBufferAddOn]			  ;
	mov	esi,fileName				  ;
	mov	al,byte[esi]				  ;
	cmp	al,'.'					  ;
	jne	NotAdot 				  ;
	mov	al,byte[esi+1]				  ;
	cmp	al,' '					  ;
	je	LoopCycle				  ;
	mov	al,18h					  ;
	stosb						  ;
	jmp	YesAdot 				  ;
  NotAdot:						  ;
	mov	al,9h					  ;
	stosb						  ;
  YesAdot:						  ;
	mov	al,' '					  ;
	stosb						  ;
	mov	esi,fileName				  ;
	mov	ecx,13					  ;
	rep	movsb					  ;
	add	[ListBufferAddOn],15			  ;
	mov	edi,fileName				  ;
	mov	al,0					  ;
	mov	ecx,14					  ;
	rep	stosb					  ;
	inc	dword[File_Count]			  ;
	cmp	dword[File_Count],DefaultListNumber	  ;
	jae	GetListError2				  ;
	jmp	LoopCycle				  ; continue searching
;=======================================================  ;
;  Now load the file list.                                ;
;=======================================================  ;
DirListFinish:						  ;
	mov	edx,FileNames1				  ;
	mov	ecx,3dh
	mov	eax,16					  ;
	call	[Fatmod]				  ; Find_First
	jnc	FoundAfileOK				  ;
	cmp	al,18					  ; No More Files error code check
	je	GetListOK				  ;
	jmp	GetListError2				  ; jump if carry flag is set
FoundAfileOK:						  ;
;=======================================================  ;
;  its a file.                                            ;
;=======================================================  ;
	mov	esi,DTAbuffer +15h			  ;
	mov	al,byte[esi]				  ;
	cmp	al,10h					  ;
	je	LoopCycle2				  ;
	mov	edi,ListBuffer				  ;
	add	edi,[ListBufferAddOn]			  ;
	mov	al,7h					  ;
	stosb						  ;
	mov	al,' '					  ;
	stosb						  ;
	push	edi					  ;
	mov	edi,fileName				  ;
	call	[LowerCase]				  ;
	pop	edi					  ;
	mov	esi,fileName				  ;
	mov	ecx,13					  ;
	rep	movsb					  ;
	add	[ListBufferAddOn],15			  ;
	mov	edi,fileName				  ;
	mov	al,0					  ;
	mov	ecx,14					  ;
	rep	stosb					  ;
	inc	dword[File_Count]			  ;
	cmp	dword[File_Count],DefaultListNumber	  ;
	jae	GetListError2				  ;
;=======================================================  ;
; continue searching.                                     ;
;=======================================================  ;
	jmp	LoopCycle2				  ;        
;=======================================================  ;
; main file list loop.                                    ;
;=======================================================  ;
  LoopCycle2:						  ;
	mov	edi,fileName				  ;
	mov	al,0					  ;
	mov	ecx,14					  ;
	rep	stosb					  ;     
	mov	edx,FileNames1				  ; DS:DX points to file name
	mov	eax,17					  ;
	call	[Fatmod]				  ;                                                          ;
	jc	GetListOK				  ;
	mov	esi,DTAbuffer +15h			  ;
	mov	al,byte[esi]				  ;
	cmp	al,10h					  ;
	je	LoopCycle2				  ;
;=======================================================  ;
;  its a file.                                            ;
;=======================================================  ;
	mov	edi,ListBuffer				  ;
	add	edi,[ListBufferAddOn]			  ;
	mov	al,7h					  ;
	stosb						  ;
	mov	al,' '					  ;
	stosb						  ;
	push	edi					  ;
	mov	edi,fileName				  ;
	call	[LowerCase]				  ;
	pop	edi					  ;
	mov	esi,fileName				  ;
	mov	ecx,13					  ;
	rep	movsb					  ;
	add	[ListBufferAddOn],15			  ;
	mov	edi,fileName				  ;
	mov	al,0					  ;
	mov	ecx,14					  ;
	rep	stosb					  ;
	inc	dword[File_Count]			  ;
	cmp	dword[File_Count],DefaultListNumber	  ;
	jae	GetListError2				  ;
;=======================================================  ;
; continue searching.                                     ;
;=======================================================  ;
	jmp	LoopCycle2				  ;                     
  harddrive_free_ret:					  ;
	mov	edi,fileName				  ;
	mov	al,0					  ;
	mov	ecx,13					  ;
	rep	movsb					  ;
GetListOK:						  ; exit ok
;=======================================================  ;
;  change to back to default dir.                         ;
;=======================================================  ;
	mov	edx,TempBuff1				  ;
	mov	eax,7					  ;
	call	[Fatmod]				  ; Set_Current_Directory
	jc	GetListError				  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	esi,ListBuffer				  ;
	mov	edi,ListBuffer				  ;
	mov	ebx,dword[File_Count]			  ;
	clc						  ;
	ret						  ;
GetListError:						  ;
	mov	byte[ErrorCode],al			  ; error
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	al,byte[ErrorCode]			  ;
	stc						  ;
	ret						  ;
GetListError2:						  ; error2
	push	eax					  ;
	mov	edx,TempBuff1				  ;
	mov	eax,7					  ;
	call	[Fatmod]				  ; Set_Current_Directory
	pop	eax					  ;
	mov	byte[ErrorCode],al			  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	al,byte[ErrorCode]			  ;
	stc						  ;
	ret						  ;


;=======================================================  ;
;  change dir.                                            ;
;=======================================================  ;             
ChangeDir:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	eax,7					  ;
	call	[Fatmod]				  ; Set_Current_Directory
	jc	ChangeDirError				  ; jump if carry flag is set
ChangeDirOK:						  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	ret						  ;
ChangeDirError: 					  ;
	mov   byte[ErrorCode],al			  ;
	pop   ds					  ;
	pop   es					  ;
	popad						  ;
	mov   al,byte[ErrorCode]			  ;
	stc						  ;
	ret						  ;


;=======================================================  ;
; DrawAbox.                                               ;
;=======================================================  ;  
; CL = color                                              ;
; AL = Box start x                                        ;
; AH = Box start y                                        ;
; DL = Box x                                              ;
; DH = Box y                                              ;
							  ;
BoxColor  dd 0						  ;
BoxStartX dd 0						  ;
BoxStartY dd 0						  ;
BoxX	  dd 0						  ;
BoxY	  dd 0						  ;
DrawAbox:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	xor	ebx,ebx 				  ;
	mov	bl,cl					  ;
	mov	dword[BoxColor],ebx			  ;
	inc	al					  ;
	mov	bl,al					  ;
	mov	dword[BoxStartX],ebx			  ;
	mov	bl,ah					  ;
	mov	dword[BoxStartY],ebx			  ;
	mov	bl,dl					  ;
	mov	dword[BoxX],ebx 			  ;
	mov	bl,dh					  ;
	mov	dword[BoxY],ebx 			  ;
	xor	ebx,ebx 				  ;
	mov	ebx,dword[BoxStartY]			  ;
	mov	ecx,dword[BoxY] 			  ;
LoopDrawBox:						  ;
	push	ecx					  ;
	mov	edi,0xb8000				  ;
	add	edi,[Ytable+ebx*4]			  ;
	add	bx,1					  ;
	add	edi,dword[BoxStartX]			  ;
	mov	ecx,dword[BoxX] 			  ;
	mov	eax,dword[BoxColor]			  ;
LoopDrawLine:						  ;
	mov	[es:edi],al				  ;
	add	edi,2					  ;
	loop	LoopDrawLine				  ;
	pop	ecx					  ;
	loop	LoopDrawBox				  ;
	pop	ds					  ;               
	pop	es					  ;               
	popad						  ;               
	clc						  ;      
	ret						  ;       
