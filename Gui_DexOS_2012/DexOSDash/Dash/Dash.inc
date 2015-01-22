;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.06                                                       ;;
;; Author           :   Craig Bamford (a.k.a Dex)                                   ;;
;; Website          :   www.dex-os.com                                              ;;
;; Forum            :   http://dex.7.forumer.com                                    ;;
;; Date             :   Mar 29, 2012                                                ;;
;; Filename         :   Dash.inc                                                    ;;
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

TestBGLoaded  db  0					  ;
align 4 						  ;
;=======================================================  ;
;  DrawBackGround                                         ;
;=======================================================  ;
DrawBackGround: 					  ;
	pushad						  ;
	mov	eax,BackGroundImage			  ;
	cmp	eax,1					  ;
	je	LoadBgImage				  ;
  Fade_the_background:					  ;
;=======================================================  ;
;  Fade the background                                    ;
;=======================================================  ;
	call	Fade_screen				  ;
	jmp	DrawBackGroundExit			  ;
  LoadBgImage:						  ;
	cmp	byte[TestBGLoaded],1			  ;
	je	BGLoaded				  ;
;=======================================================  ;
;  Decode jpeg                                            ;
;=======================================================  ;
	mov	edi,BackGroundBuffer			  ; Image dest (uncompressed)
	mov	esi,file_area				  ; Image source (compressed)
	mov	ecx,FileEnd-file_area			  ; Image size (compressed)
	call	DeCodeJpeg				  ; call function
	jc	Fade_the_background			  ; CF flag set on error
	mov	byte[TestBGLoaded],1			  ; Set BG loaded OK flag
  BGLoaded:						  ;
;=======================================================  ;
;  PutImage                                               ;
;=======================================================  ;
	mov	ecx,0x00ffffff				  ; fill color
	mov	edi,VesaBuffer				  ; dest buff
	mov	esi,BackGroundBuffer			  ; souce buff
	call	PutImageFill32				  ; call put image
DrawBackGroundExit:					  ;
	popad						  ;
	ret						  ;
							  ;
MenuBarBG_X  = 800					  ;
MenuBarBG_Y  = 100					  ;
;=======================================================  ;
;  SaveMenuBarBG                                          ;
;=======================================================  ;
SaveMenuBarBG:						  ;
	pushad						  ;
	xor	eax,eax 				  ; boxX
	mov	ebx,MenuStart				  ; boxY
	mov	edi,MenBar800_Store_Buffer		  ;
	mov	ecx,MenuBarBG_Y 			  ; hight
	mov	edx,MenuBarBG_X 			  ; width
	call	SaveFilledBoxVesa			  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  RestoreMenuBarBG                                       ;
;=======================================================  ;
RestoreMenuBarBG:					  ;
	pushad						  ;
	xor	eax,eax 				  ; boxX
	mov	ebx,MenuStart				  ; boxY
	mov	esi,MenBar800_Store_Buffer		  ;
	mov	ecx,MenuBarBG_Y 			  ; hight
	mov	edx,MenuBarBG_X 			  ; width
	call	ReStoreFilledBoxVesa			  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  DrawMainMenu.                                          ;
;=======================================================  ;
DrawMainMenu:						  ;
	pushad						  ;
	call	DrawBackGround				  ;
	call	SaveMenuBarBG				  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  BottomMenu.                                            ;
;=======================================================  ;
BottomMenu:						  ;
	pushad						  ;
;=======================================================  ;
;  Decode jpeg                                            ;
;=======================================================  ;
	mov	edi,BottonMenu_Buffer			  ; Image dest (uncompressed)
	mov	esi,file_area_BottomMenu		  ; Image source (compressed)
	mov	ecx,FileEnd_BottomMenu-file_area_BottomMenu  ; Image size (compressed)
	call	DeCodeJpeg				  ; call function
	popad						  ;
	ret						  ;
							  ;
MenuBAR db 1						  ;
;=======================================================  ;
;  DrawMenuBar                                            ;
;=======================================================  ;
DrawMenuBar:						  ;
	pushad						  ;
	cmp    [MenuBAR],1				  ;
	jne    Bar2					  ;
  Bar1: 						  ;
	call   RestoreMenuBarBG 			  ;
	jmp    DrawMenuBarExit				  ;
  Bar2: 						  ;
	cmp    [MenuBAR],2				  ;
	jne    DrawMenuBarExit				  ;
	call   SaveMenuBarBG				  ;
	mov    edi,VesaBuffer				  ;
	mov    esi,BottonMenu_Buffer			  ;
	xor    eax,eax					  ;
	mov    ecx,500					  ;
	call   PutImage32				  ;
	jmp    DrawMenuBarExit				  ;
DrawMenuBarExit:					  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  SetFolder.                                             ;
;=======================================================  ;
SetFolder:						  ;
;=======================================================  ;
;  Decode jpeg                                            ;
;=======================================================  ;
	pushad						  ;
	mov	edi,Arrow_Buffer			  ; Image dest (uncompressed)
	mov	esi,file_area_arrow			  ; Image source (compressed)
	mov	ecx,FileEnd_arrow-file_area_arrow	  ; Image size (compressed)
	call	DeCodeJpeg				  ; call function
							  ;
	mov	edi,Folder_Buffer			  ; Image dest (uncompressed)
	mov	esi,file_area_folder			  ; Image source (compressed)
	mov	ecx,FileEnd_folder-file_area_folder	  ; Image size (compressed)
	call	DeCodeJpeg				  ; call function
							  ;
	mov	edi,App_Buffer				  ; Image dest (uncompressed)
	mov	esi,file_area_App			  ; Image source (compressed)
	mov	ecx,FileEnd_App-file_area_App		  ; Image size (compressed)
	call	DeCodeJpeg				  ; call function
	popad						  ;
	ret						  ;
							  ;
BlackMenuStartX = 202					  ;
BlackMenuStartY = 90					  ;
MenuBG_Y	= 384					  ;
MenuBG_X	= 400					  ;
GetListBufferAddress	   dd 0 			  ;
GetListBufferAddressAddOn  dd 0 			  ;
GetListFile_Count	   dd 0 			  ;
DrawFolderX		   dd 0 			  ;
DrawFolderY		   dd 0 			  ;
;=======================================================  ;
;   LoadMenu.                                             ;
;=======================================================  ;       
LoadMenu:						  ;
	pushad						  ;
	mov	eax,BlackMenuStartX			  ; start X
	mov	ecx,BlackMenuStartY			  ; start Y 
	mov	esi,Menu_Buffer 			  ; source image
	mov	edi,VesaBuffer				  ; dest image
	call	PutImageAlpha32 			  ; call function
	call	GetList 				  ;
	jc	GetList_Error				  ;
  GetList_OK:						  ;
	mov	word[MainWindowX],0			  ;                 
	mov	word[MainWindowY],0			  ;
	mov	dword[GetListBufferAddressAddOn],0	  ;
	add	esi,dword[BarAddOn_3]			  ;
	mov	dword[GetListBufferAddress],esi 	  ;
	mov	dword[GetListFile_Count],ebx		  ;
	mov	bx,250					  ;
	shl	ebx,16					  ;
	mov	dword[DrawFolderX],250-16		  ;
	mov	bx,110					  ;
	mov	ecx,16					  ;
	mov	dword[DrawFolderY],109			  ;
align 4 						  ;
  GetListLoop:						  ;
	push	ecx					  ;
	mov	ecx,0x005d6864				  ;
	mov	edx,dword[GetListBufferAddress] 	  ;
	add	edx,dword[GetListBufferAddressAddOn]	  ;
	mov	eax,2					  ;
	add	bx,20					  ;
	add	dword[DrawFolderY],20			  ;
	cmp	byte[edx],18h				  ;
	jne	Try_Folder				  ;
	call	DrawArrow				  ;
	jmp	ItsAfolder				  ;
  Try_Folder:						  ;
	cmp	byte[edx],9h				  ;
	jne	ItsAapp 				  ;
	call	DrawFolder				  ;
	jmp	ItsAfolder				  ;
  ItsAapp:						  ;
	cmp	byte[edx],7h				  ;
	jne	ItsAfolder				  ;
	call	DrawApp 				  ;
  ItsAfolder:						  ;
	call	WRITE_TEXT_TO_WINDOW_04_OLD		  ;
	add	dword[GetListBufferAddressAddOn],15	  ;
	pop	ecx					  ;
	loop	GetListLoop				  ;
GetList_Error:						  ;
	popad						  ;
	ret						  ;
							  ;
;=======================================================  ;
;  DrawFolder                                             ;
;=======================================================  ;       
DrawFolder:						  ;
	pushad						  ;
	mov    edi,VesaBuffer				  ;
	mov    esi,Folder_Buffer			  ;
	mov    eax,dword[DrawFolderX]			  ;
	mov    ecx,dword[DrawFolderY]			  ;
	call   PutImage32				  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  DrawApp                                                ;
;=======================================================  ;   
DrawApp:						  ;
	pushad						  ;
	mov    edi,VesaBuffer				  ;
	mov    esi,App_Buffer				  ;
	mov    eax,dword[DrawFolderX]			  ;
	mov    ecx,dword[DrawFolderY]			  ;
	call   PutImage32				  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  DrawArrow                                              ;
;=======================================================  ; 
DrawArrow:						  ;
	pushad						  ;
	mov    edi,VesaBuffer				  ;
	mov    esi,Arrow_Buffer 			  ;
	mov    eax,dword[DrawFolderX]			  ;
	mov    ecx,dword[DrawFolderY]			  ;
	call   PutImage32				  ;
	popad						  ;
	ret						  ;
							  ;
BarAddOn	 dd 0					  ;
BarAddOn_1	 dd 0					  ;
BarAddOn_2	 dd 0					  ;
BarAddOn_3	 dd 0					  ;
List_Menu_Number dd 0					  ;
;=======================================================  ;
;  DrawMenu_Bar                                           ;
;=======================================================  ;  
DrawMenu_Bar:						  ;
	pushad						  ;
	mov	eax,250-20				  ; start X
	mov	ecx,122 				  ; start Y 
	add	ecx,dword[BarAddOn]			  ;
	mov	esi,Bar_Buffer				  ; source image
	mov	edi,VesaBuffer				  ; dest image
	call	PutImageAlpha32 			  ; call function
	popad						  ;
	ret						  ;
							  ;
DefaultListNumber = 1024				  ;
;=======================================================  ;
;  File data                                      *New*   ;
;=======================================================  ;
    drive	   db 0 				  ;
    DriveNew	   db 0 				  ;
		   db ' is $'				  ;
    Drive2	   db 0 				  ;
		   db ':\$'				  ;
ChangeDirTest	   db 0 				  ;
ListBufferAddOn    dd 0 				  ;
File_Count	   dd 0 				  ;
 FileNames1	   db '*.gex',0 			  ; file name
FileNames	   db '*',0				  ; file name
;=======================================================  ;
;  DTA Buffer                                             ;
;=======================================================  ;
    DTAbuffer:	   db 22 dup(?) 			  ; header info - not used
    fileTime:	   dw 0 				  ; time
    fileDate:	   dw 0 				  ; date
    fileSize:	   dd 0 				  ; size
    fileName:	   db 14 dup(0) 			  ; name
    fileExten:	   db 3 dup(0)
    EndOfString:   db 0,0, '$'				  ;
    TempBuff1:	   db 68 dup(' ')			  ;
		   db 0 				  ;
ErrorCode db 0						  ;
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
	;call   ChangeDir                                 ;
  NoArguements: 					  ;
;=======================================================  ;
;  get current drive.                                     ;
;=======================================================  ;
	mov	eax,2					  ;
	call	[Fatmod]				  ;
	inc	al					  ;
	mov	byte [drive],al 			  ;
	add	al,40h					  ;
	mov	byte[DriveNew],al			  ;
	mov	byte[Drive2],al 			  ;
;=======================================================  ;
;  redirect DTA to new buffer                             ;
;=======================================================  ;
	mov	edx,DTAbuffer				  ;
	mov	eax,3					  ;
	call	[Fatmod]				  ; SetDTA_Address
							  ;
	mov	edx,FileNames				  ;
	mov	ecx,3dh 				  ; = do not show hidden files; show all files = 3fh
	mov	eax,16					  ;
	call	[Fatmod]				  ; Find_First
	jc	GetListError2				  ; jump if carry flag is set
;=======================================================  ;
;  test for a volume name.                                ;
;=======================================================  ;
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
	mov	eax,17					  ;
	call	[Fatmod] ;Find_Next                       ;
	jc	DirListFinish				  ; try files, if carry flag is set
	mov	esi,DTAbuffer +15h			  ;
	mov	al,byte[esi]				  ;
	test	al,10h					  ;
	jnz	@f					  ;
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
	mov	ecx,21h 				  ; = do not show hidden files; show all files = 3fh
	mov	eax,16					  ;
	call	[Fatmod]				  ; Find_First
	jnc	FoundAfileOK				  ;
	cmp	al,18					  ; No More Files error code check
	je	GetListOK				  ;
	jmp	GetListError2				  ; jump if carry flag is set
FoundAfileOK:						  ;
	mov	esi,DTAbuffer +15h			  ;
	mov	al,byte[esi]				  ;
	cmp	al,10h					  ;
	je	LoopCycle2				  ;
	cmp	word[fileExten],'GE'			  ;-
	jne	LoopCycle2				  ;-
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
	call	[Fatmod] ;Find_Next                       ;
							  ;
	jc	GetListOK				  ;
	mov	esi,DTAbuffer +15h			  ;
	mov	al,byte[esi]				  ;
	cmp	al,10h					  ;
	je	LoopCycle2				  ;
	cmp	word[fileExten],'GE'			  ;-
	jne	LoopCycle2				  ;-
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
;  SaveBackGroundMenu.                                    ;
;=======================================================  ;     
SaveBackGroundMenu:					  ;
	pushad						  ;
	mov	eax,BlackMenuStartX			  ; boxX
	mov	ebx,BlackMenuStartY			  ; boxY
	mov	edi,Menu_Store_Buffer			  ;
	mov	ecx,MenuBG_Y				  ; hight
	mov	edx,MenuBG_X				  ; width
	call	SaveFilledBoxVesa			  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  RestoreBackGroundMenu.                                 ;
;=======================================================  ;        
RestoreBackGroundMenu:					  ;
	pushad						  ;
	mov	eax,BlackMenuStartX			  ; boxX
	mov	ebx,BlackMenuStartY			  ; boxY
	mov	esi,Menu_Store_Buffer			  ;
	mov	ecx,MenuBG_Y				  ; hight
	mov	edx,MenuBG_X				  ; width
	call	ReStoreFilledBoxVesa			  ;
	call	BuffToScreen				  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  RestoreBackGroundMenu No buffer to screen.             ;
;=======================================================  ;        
RestoreBackGroundMenuNB:				  ;
	pushad						  ;
	mov	eax,BlackMenuStartX			  ; boxX
	mov	ebx,BlackMenuStartY			  ; boxY
	mov	esi,Menu_Store_Buffer			  ;
	mov	ecx,MenuBG_Y				  ; hight
	mov	edx,MenuBG_X				  ; width
	call	ReStoreFilledBoxVesa			  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  SaveMenulist.                                          ;
;=======================================================  ;            
SaveMenulist:						  ;
	pushad						  ;
	mov	eax,BlackMenuStartX			  ; boxX
	mov	ebx,BlackMenuStartY			  ; boxY
	mov	edi,Menu_List_Buffer			  ;
	mov	ecx,MenuBG_Y				  ; hight
	mov	edx,MenuBG_X				  ; width
	call	SaveFilledBoxVesa			  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  RestoreMenuList.                                       ;
;=======================================================  ;        
RestoreMenuList:					  ;
	pushad						  ;
	mov	eax,BlackMenuStartX			  ; boxX
	mov	ebx,BlackMenuStartY			  ; boxY
	mov	esi,Menu_List_Buffer			  ;
	mov	ecx,MenuBG_Y				  ; hight
	mov	edx,MenuBG_X				  ; width
	call	ReStoreFilledBoxVesa			  ;
	popad						  ;
	ret						  ;
							  ;
ChangeOK db 0						  ;
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
;=======================================================  ;
;  Exit OK.                                               ;
;=======================================================  ;
  ChangeDirLoadOk:					  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	ret						  ;
;=======================================================  ;
;  Exit Error.                                            ;
;=======================================================  ;
FddLoadError:						  ;
	;mov   byte[ErrorCode],al                         ;
	pop   ds					  ;
	pop   es					  ;
	popad						  ;
	;mov   al,byte[ErrorCode]                         ;
	stc						  ;
	ret						  ;
							  ;
;=======================================================  ;
;  SpeedUpKeyBoard.                                       ;
;=======================================================  ;
SpeedUpKeyBoard:					  ;
	push	eax					  ;
	mov	al,0xf3 				  ;          
	out	0x60,al 				  ;           
KeyBoardWait:						  ;
	in	al,0x64 				  ;
	test	al,10b					  ;           
	jne	KeyBoardWait				  ;           
	mov	al,1					  ;
	and	al,111b 				  ;
	out	0x60,al 				  ;                   
	pop	eax					  ;
	ret						  ;

;=======================================================  ;
; open.                                          *fasm*   ;
; in                                                      ;
;    edx = zero-ended file name                           ;
;                                                         ;
; out                                                     ;
;     ebx = file handle                                   ;
;=======================================================  ;
open_FatMod:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	push	eax					  ;
	mov	ax,18h					  ;
	mov	es,ax					  ;
	mov	ds,ax					  ;
	pop	eax					  ;
	mov	eax,9					  ;
	call	[Fatmod]				  ;
	jc	file_error				  ;
	xor	ebx,ebx 				  ;
	mov	bx,ax					  ;
	mov	dword[FileHandleNumer] ,ebx		  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	ebx,[FileHandleNumer]			  ;
	clc						  ;
	ret						  ;
							  ;
file_error:						  ;
	mov	[ErrorCode],al				  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	mov	ah,[ErrorCode]				  ;
	stc						  ;
	ret						  ;
FileHandleNumer dd 0					  ;
NumberOfBytesRed dd 0					  ;
;=======================================================  ;
; read.                                          *fasm*   ;
;                                                         ;
; ebx file handle                                         ;
; ecx count of bytes to read                              ;
; edx pointer to buffer                                   ;
;=======================================================  ;
read_FatMod:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ax,18h					  ;
	mov	es,ax					  ;
	mov	ds,ax					  ;
	mov	dword[NumberOfBinFile],ecx		  ;
	mov	dword[BufferVar],edx			  ;
	mov	dword[Fhandler],ebx			  ;
;=======================================================  ;
;  Read from file.                                        ;
;=======================================================  ;
  Read_FileMainLoop:					  ;
	mov	ebx,dword[Fhandler]			  ;
	mov	ecx,dword[NumberOfBinFile]		  ;
	mov	edx,dword[BufferVar]			  ;
							  ;
	mov	eax,11					  ; Read file
	call	[Fatmod]				  ;
	jc	read_error				  ;
  read_ok:						  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	clc						  ;
	ret						  ;
							  ;
  read_error:						  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	stc						  ;
	ret						  ;
							  ;
;=======================================================  ;
; close.                                         *fasm*   ;
;=======================================================  ;
close_FatMod:						  ;
	pushad						  ;
	push	es					  ;
	push	ds					  ;
	mov	ax,18h					  ;
	mov	es,ax					  ;
	mov	ds,ax					  ;
	mov	dword[Fhandler],ebx			  ;
;=======================================================  ;
;  exit, close dest file.                                 ;
;=======================================================  ;
	mov	ebx,dword[Fhandler]			  ;
	mov	eax,10					  ;
	call	[Fatmod]				  ;
  close_ok_FatMod:					  ;
	pop	ds					  ;
	pop	es					  ;
	popad						  ;
	ret						  ;
							  ;
BufferVar	 dd 0					  ;
NumberOfBinFile  dd 0					  ;
Fhandler	 dd 0					  ;
