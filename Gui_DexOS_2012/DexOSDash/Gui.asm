;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;                                                                                  ;;
;; Project          :   DexOS                                                       ;;
;; Ver              :   00.06                                                       ;;
;; Author           :   Craig Bamford (a.k.a Dex)                                   ;;
;; Website          :   www.dex-os.com                                              ;;
;; Forum            :   http://dex.7.forumer.com                                    ;;
;; Date             :   Mar 29, 2012                                                ;;
;; Filename         :   Gui.inc                                                     ;;
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
MaxFileSize	=	 1024*1024*10			  ;
BackGroundImage = 1					  ; 1 = background image, 0 = Fade background
SkinTextColor	= 0x0052ba0d				  ; the text color for this skin limegreen (lighter green = 0x0093e65c)
ButtonColor	= 0x00111213				  ;
MenuStart	= 500					  ; NOTE: CHANGE THIS TO 225 for 1024*768 mode
format binary as 'bin'					  ;
use32							  ;
	ORG   0x800000					  ;* where our program is loaded to NOTE Its not 0x400000
	jmp   start					  ; jump to the start of program.
	db    'GUI6'					   ; We check for this, to make shore it a valid Dex4u file.
;=======================================================  ;
;  Start of program.                                      ;
;=======================================================  ;
start:							  ;
	mov   ax,18h					  ;
	mov   ds,ax					  ;
	mov   es,ax					  ;
;=======================================================  ;
;  Get calltable address.                                 ;
;=======================================================  ;
	mov   edi,Functions				  ; this is the interrupt
	mov   al,0					  ; we use to load the DexFunction.inc
	mov   ah,0x0a					  ; with the address to DexOS functions.
	int   50h					  ;
;=======================================================  ;
;  Test if fat module is loaded                           ;
;=======================================================  ;
	mov    esi,ID					  ;
	call   [ModuleFunction] 			  ;
	jc     NoFatMod 				  ;
	mov    [Fatmod],eax				  ;
NoFatMod:						  ;
;-------------------------------------------------------  ;
; Do realmode int (vesa).                                 ;
; 0x4112 =  640 , 480 , 0                                 ;
; 0x4115 =  800 , 600 , 0                                 ;
; 0x4118 = 1024 , 768 , 0                                 ;
; 0x411B = 1280 ,1024 , 0                                 ;
;=======================================================  ;
;   set vesa up                                           ;
;=======================================================  ;
	mov    ax,4f00h 				  ;
	mov    bx,0x4115				  ;
	mov    edi,Mode_Info				  ;
	call   [RealModeInt10h] 			  ;
	jc     VesaErrorM2D				  ;
;=======================================================  ;
;   Setup vesa mouse.                                     ;
;=======================================================  ;
	;call  SetUpMouseVesa                             ;
	;jc    MouseErrorM2D                              ;
;=======================================================  ;
;   Setup .VesaBuffer address                             ;
;=======================================================  ;
	mov   esi,VesaBuffer				  ;
	call  VesaBufferSetM2D				  ;
;=======================================================  ;
;  Decode Dif                                    Menu     ;
;=======================================================  ;
	mov   esi,file_area_Menu			  ;
	mov   edi,Menu_Buffer				  ;
	mov   ecx,BufferTemp				  ;
	call  DIFdecoder				  ;
	jc    VesaErrorM2D				  ;
;=======================================================  ;
;  Decode Dif                                     Bar     ;
;=======================================================  ;
	mov   esi,file_area_Bar 			  ;
	mov   edi,Bar_Buffer				  ;
	mov   ecx,BufferTemp				  ;
	call  DIFdecoder				  ;
	jc    VesaErrorM2D				  ;
;=======================================================  ;
;   Set vesa mode 24 or 32bit                             ;
;=======================================================  ;
	call  SetVesaModeSys				  ;
;=======================================================  ;
;   SpeedUp KeyBoard                                      ;
;=======================================================  ;
	call  SpeedUpKeyBoard				  ;
;=======================================================  ;
;   Draw BackGround                                       ;
;=======================================================  ;
	call  DrawMainMenu				  ;
;=======================================================  ;
;   Draw BottomMenu                                       ;
;=======================================================  ;
	call  BottomMenu				  ;
;=======================================================  ;
;   Set app/folder image                                  ;
;=======================================================  ;
	call  SetFolder 				  ;
;=======================================================  ;
;   hook int40h                                           ;
;=======================================================  ;
	call  Hook_int40h				  ;
;=======================================================  ;
;   Go to second start                                    ;
;=======================================================  ;
	jmp   START2					  ;
;=======================================================  ;
;   ** Vesa mouse setup error **                          ;
;=======================================================  ;
MouseErrorM2D:						  ;
       ; call  [SetMouseOff]                              ;
;=======================================================  ;
;   return to text mode                                   ;
;=======================================================  ;
	mov    ax,03h					  ;  move the number of the mode to ax
	call   [RealModeInt10h] 			  ;  and enter the mode using int 10h
	xor    eax,eax					  ;
	call   [SetCursorPos]				  ;
	ret						  ;
;=======================================================  ;
;   ** Vesa mouse setup error **                          ;
;=======================================================  ;
VesaErrorM2D:						  ; you can print error message if you have the fonts inc
;=======================================================  ;
;   return to text mode                                   ;
;=======================================================  ;
	mov    ax,03h					  ;  move the number of the mode to ax
	call   [RealModeInt10h] 			  ;  and enter the mode using int 10h
	xor    eax,eax					  ;
	call   [SetCursorPos]				  ;
	ret						  ;
START2: 						  ;
	mov   [MenuBAR],1				  ;
	call  BuffToScreen				  ;
	mov   eax,18					  ;
	call  [SetDelay]				  ;
noclose:						  ;
	mov   ebx,3					  ;
	cmp   [speed_flag],1				  ;
	jne   .skip					  ;
	call  CHECK_FOR_EVENT_NO_WAIT_11		  ;
	jmp   .skip2					  ;
.skip:							  ;
	call  WAIT_FOR_EVENT_WITH_TIMEOUT_23		  ;
.skip2: 						  ;
	cmp   eax,1					  ;
	je    noclose					  ;
	cmp   eax,2					  ;
	je    keytest					  ;
	cmp   eax,3					  ;
	je    noclose					  ;
	jmp   noclose					  ;
keytest:						  ;
	call  GET_KEY_02				  ;
	cmp   bl,0x4d					  ;
	jne   TryLeftArrow				  ;
	cmp   [MenuBAR],2				  ;
	jb    MoveBarRight				  ;
	mov   [MenuBAR],1				  ;
	call  DrawMenuBar				  ;
	call  BuffToScreen				  ;
	jmp   MenuKeyTestExit				  ;
MoveBarRight:						  ;
	inc   [MenuBAR] 				  ;
	call  DrawMenuBar				  ;
	call  BuffToScreen				  ;
	jmp   MenuKeyTestExit				  ;
TryLeftArrow:						  ;
	cmp   bl,0x4b					  ;
	jne   TryEnterKey				  ;
	cmp   [MenuBAR],1				  ;
	ja    MoveBarLeft				  ;
	mov   [MenuBAR],2				  ;
	call  DrawMenuBar				  ;
	call  BuffToScreen				  ;
	jmp   MenuKeyTestExit				  ;
MoveBarLeft:						  ;
	dec   [MenuBAR] 				  ;
	call  DrawMenuBar				  ;
	call  BuffToScreen				  ;
	jmp   MenuKeyTestExit				  ;
TryEnterKey:						  ;
	cmp   bl,0x1c					  ;
	jne   MenuKeyTestExit				  ;
	cmp   [MenuBAR],1				  ;
	jne   CommandLine				  ;
	call  Test_Menu 				  ;
	jmp   MenuKeyTestExit				  ;
CommandLine:						  ;
	cmp   [MenuBAR],2				  ;
	jne   TryTurnOff				  ;
	jmp   CLOSE_THIS_PROGRAM_x1			  ;
TryTurnOff:						  ;
	jmp   MenuKeyTestExit				  ;
							  ;
MenuKeyTestExit:					  ;
	jmp	noclose 				  ;
MenTestExit:						  ;
	; close this program                              ;
	jmp   CLOSE_THIS_PROGRAM_x1			  ;
Test_Menu:						  ;
	pushad						  ;
	mov	dword[BarAddOn_1],0			  ;
	mov	dword[BarAddOn],0			  ;
	mov	dword[BarAddOn_2],0			  ;
	mov	dword[BarAddOn_3],0			  ;
	call	SaveBackGroundMenu			  ;
	call	LoadMenu				  ;
	call	SaveMenulist				  ;
	call	DrawMenu_Bar				  ;
	call	BuffToScreen				  ;
	cmp	dword[GetListFile_Count],15		  ;
	jae	MoreThan15				  ;
	mov	ebx,dword[GetListFile_Count]		  ;
	cmp	ebx,0					  ;
	je	YesZero 				  ;
	dec	ebx					  ;
YesZero:						  ;
	mov	dword[List_Menu_Number],ebx		  ;
	jmp	Menu_Key_TestExit			  ;
MoreThan15:						  ;
	mov	dword[List_Menu_Number],15		  ;
align 4 						  ;
Menu_Key_TestExit:					  ;
	call	[WaitForKeyPress]			  ;
	cmp	al,0x1b 				  ;
	je	Menu_Exit				  ;
	cmp	ah,0x50 				  ;
	jne	TryUpArrow_L_Menu			  ;
	mov	ebx,dword[List_Menu_Number]		  ;
	cmp	dword[BarAddOn_1],ebx			  ;
	jb	MoveBarDown_L_Menu			  ;
	cmp	dword[GetListFile_Count],15		  ;
	ja	UpDateKeyVar				  ;
	jmp	Menu_Key_TestExit			  ;
UpDateKeyVar:						  ;
	mov	ebx,dword[GetListFile_Count]		  ;
	dec	ebx					  ;
	cmp	dword[BarAddOn_2],ebx			  ;
	jae	Menu_Key_TestExit			  ;
	inc	dword[BarAddOn_2]			  ;
	add	dword[BarAddOn_3],15			  ;
	call	LoadMenu				  ;
	call	SaveMenulist				  ;
	call	DrawMenu_Bar				  ;
	call	BuffToScreen				  ;
	jmp	Menu_Key_TestExit			  ;
MoveBarDown_L_Menu:					  ;
	inc	dword[BarAddOn_1]			  ;
	inc	dword[BarAddOn_2]			  ;
	add	dword[BarAddOn],20			  ;
	call	RestoreMenuList 			  ;
	call	DrawMenu_Bar				  ;
	call	BuffToScreen				  ;
	jmp	Menu_Key_TestExit			  ;
TryUpArrow_L_Menu:					  ;
	cmp	ah,0x48 				  ;
	jne	Try_Enter_Key				  ;
	cmp	dword[BarAddOn],1			  ;
	ja	Move_Bar_Up				  ;
	cmp	dword[BarAddOn_3],0			  ;
	jbe	Menu_Key_TestExit			  ;
	dec	dword[BarAddOn_2]			  ;
	sub	dword[BarAddOn_3],15			  ;
	call	LoadMenu				  ;
	call	SaveMenulist				  ;
	call	DrawMenu_Bar				  ;
	call	BuffToScreen				  ;
	jmp	Menu_Key_TestExit			  ;
Move_Bar_Up:						  ;
	dec	dword[BarAddOn_2]			  ;
	dec	dword[BarAddOn_1]			  ;
	sub	dword[BarAddOn],20			  ;
	call	RestoreMenuList 			  ;
	call	DrawMenu_Bar				  ;
	call	BuffToScreen				  ;
	jmp	Menu_Key_TestExit			  ;
Try_Enter_Key:						  ;
	cmp	ah,0x1c 				  ;
	jne	Menu_Key_TestExit			  ;
	mov	ebx,dword[BarAddOn_2]			  ;
	mov	eax,dword[BarAddOn_2]			  ;
	shl	eax,4					  ;
	sub	eax,ebx 				  ;
	mov	esi,ListBuffer				  ;dword[GetListBufferAddress]
	add	esi,eax 				  ;
	mov	al,byte[esi]				  ;
	cmp	al,7h					  ;
	je	WeHaveAfile				  ;
	call	ChangeDirLoad				  ;
	jc	FolderError				  ;
	mov	dword[BarAddOn_1],0			  ;
	mov	dword[BarAddOn],0			  ;
	mov	dword[BarAddOn_2],0			  ;
	mov	dword[BarAddOn_3],0			  ;
	call	LoadMenu				  ;
	call	SaveMenulist				  ;
	call	DrawMenu_Bar				  ;
	call	BuffToScreen				  ;
	cmp	dword[GetListFile_Count],15		  ;
	jae	MoreThan15_Folder			  ;
	mov	ebx,dword[GetListFile_Count]		  ;
	cmp	ebx,0					  ;
	je	YesZero_Folder				  ;
	dec	ebx					  ;
YesZero_Folder: 					  ;
	mov	dword[List_Menu_Number],ebx		  ;
	jmp	Menu_Key_TestExit			  ;
MoreThan15_Folder:					  ;
	mov	dword[List_Menu_Number],15		  ;
	jmp	Menu_Key_TestExit			  ;
							  ;
WeHaveAfile:						  ;
;=======================================================  ;
; Load GUI.                                               ;
;=======================================================  ;
	mov	dword[ImageLoadAddrVar],0x1A00000	  ; 26 MB
	add	esi,2					  ;
	mov	edx,esi 				  ;
	mov	[FileAddress],esi			  ;
	call	open_FatMod				  ;
	jc	FolderError				  ;
	mov	dword[FileHandle2],ebx			  ;
	mov	edx,dword[ImageLoadAddrVar]		  ;
	mov	ecx,MaxFileSize 			  ;
	call	read_FatMod				  ;
	jc	FolderError1				  ;
	mov	ebx,dword[FileHandle2]			  ;
	call	close_FatMod				  ;
;=======================================================  ;
; Test gex file.                                          ;
;=======================================================  ;
	mov	esi,dword[ImageLoadAddrVar]		  ;
	add	esi,2					  ;
	cmp	dword[ds:esi],'GEX6'			  ;
	je	ItsaDexProg_GEX 			  ;
	add	esi,3					  ;
	cmp	dword[ds:esi],'GEX6'			  ;
	jne	FolderError				  ;
;=======================================================  ;
;  Restore menu background.                               ;
;=======================================================  ;
ItsaDexProg_GEX:
	call	RestoreBackGroundMenuNB 		  ;
;=======================================================  ;
; Run GUI.                                                ;
;=======================================================  ; 
	mov	ax,18h					  ;
	mov	ds,ax					  ;
	mov	es,ax					  ;
	xor	eax,eax 				  ;
	mov	ebx,eax 				  ;
	mov	ecx,eax 				  ;
	mov	edx,eax 				  ;
	mov	esi,eax 				  ;
	mov	edi,eax 				  ;
	call	dword[ImageLoadAddrVar] 		  ;                          
	mov	ax,18h					  ;             
	mov	ds,ax					  ;
	mov	es,ax					  ;
	xor	eax,eax 				  ;
	mov	esi,eax 				  ;
	mov	edi,eax 				  ;
	mov	ebx,eax 				  ;
	mov	ecx,eax 				  ;
	mov	edx,eax 				  ;
;=======================================================  ;
; Setup .VesaBuffer address                               ;
;=======================================================  ; 
	pushad						  ;
	mov	esi,VesaBuffer				  ;
	call	VesaBufferSetM2D			  ;
	popad						  ;
	jmp	Menu_Exit1				  ;
FolderError1:						  ;
	mov	ebx,dword[FileHandle2]			  ;
	call	close_FatMod				  ;
FolderError:						  ;
	jmp	Menu_Key_TestExit			  ;
Menu_Exit:						  ;
	call	RestoreBackGroundMenuNB 		  ;
Menu_Exit1:						  ;
	call	BuffToScreen				  ;
	popad						  ;
	ret						  ;
;=======================================================  ;
;  We come here, if no fat module available               ;
;=======================================================  ;
FatmodError:						  ;
	stc						  ;
	ret						  ;
;=======================================================  ;
;  Includes                                               ;
;=======================================================  ;
include 'Men2Dex\Men2Dex.inc'				  ;
;=======================================================  ;
; Data                                                    ;
;=======================================================  ;
ID			db 'DEXOSFAT',0 		  ;
Fatmod			dd   FatmodError		  ;
FileHandle2	 dd 0					  ;
ImageLoadAddrVar dd 0					  ;
speed_flag db  1					  ;
StartMenuOpenClose	db	0			  ;
align 4 						  ;
file_area_arrow:					  ;
file   'BGimage\Arrow.jpg'				  ;
FileEnd_arrow:						  ;
rd 1							  ;
align 4 						  ;
file_area_App:						  ;
file   'BGimage\App.jpg'				  ;
FileEnd_App:						  ;
rd 1							  ;
align 4 						  ;
file_area_folder:					  ;
file   'BGimage\Folder.jpg'				  ;
FileEnd_folder: 					  ;
rd 1							  ;
align 4 						  ;
file_area:						  ;
file   'BGimage\Cod800.jpg'				  ;
FileEnd:						  ;
rd 1							  ;
align 4 						  ;
file_area_BottomMenu:					  ;
file   'BGimage\BottomMenu.jpg' 			  ;
FileEnd_BottomMenu:					  ;
rd 1							  ;
align 4 						  ;
file_area_Menu: 					  ;
file   'Skins\Menu.dif' 				  ;
file_area_Menu_FileEnd: 				  ;
rd 1							  ;
align 4 						  ;
file_area_Bar:						  ;
file   'Skins\Bar.dif'					  ;
file_area_Bar_FileEnd:					  ;
rd 1							  ;
align 4 						  ;
include 'Dif\Dif.inc'					  ;
align 4 						  ;
include 'Functions\Convert.inc' 			  ;
align 4 						  ;
include 'Mouse\MouseData.inc'				  ;
align 4 						  ;
include 'Mouse\MouseLib.inc'				  ;
align 4 						  ;
include 'Mouse\MouseDraw.inc'				  ;
align 4 						  ;
include 'Text\VesaText.inc'				  ;
align 4 						  ;
include 'Text\Font16.inc'				  ;
align 4 						  ;
include 'Text\Font8.inc'				  ;
align 4 						  ;
include 'Graphic\Graphic.inc'				  ;
align 4 						  ;
include 'Graphic\Line.inc'				  ;
align 4 						  ;
include 'Beep\Beep.inc' 				  ;
align 4 						  ;
include 'WinStructList\WinStructList.inc'		  ;
align 4 						  ;
include 'ButtonList\ButtonList.inc'			  ;
align 4 						  ;
include 'Dash\Dash.inc' 				  ;
align 4 						  ;
include 'Int40h\Int40h.inc'				  ;
align 4 						  ;
include 'Time\Time.inc' 				  ;
align 4 						  ;
include 'Jpeg\Filelib.inc'				  ;
align 4 						  ;
include 'Jpeg\Memlib.inc'				  ;
align 4 						  ;
include 'Jpeg\Jpeglib.inc'				  ;
align 4 						  ;
include 'Jpeg\JpegGraphics.inc' 			  ;
align 4 						  ;
include 'Jpeg\Jpegdat.inc'				  ;
align 4 						  ;
include 'Jpeg\Rdata.inc'				  ; NOTE: Must be last in list as contains rd, rw, rd,
align 4 						  ;
include 'Temp.inc'					  ;
align 4 						  ;
include 'Dex.inc'					  ;
align 4 						  ;
BackGroundBuffer:			rd  1024*1024+2   ; buffer for background image
align 4 						  ;
BufferTemp:				rd  1024*1024	  ;
align 4 						  ;
Menu_Buffer:				rd  400*384+2	  ;
align 4 						  ;
Bar_Buffer:				rd  32*336+2	  ;
align 4 						  ;
MenBar800_Store_Buffer: 		rd  100*800+2	  ;
align 4 						  ;
Menu_Store_Buffer:			rd  400*384+2	  ;
align 4 						  ;
Menu_List_Buffer:			rd  400*384+2	  ;
align 4 						  ;
BottonMenu_Buffer:			rd  96*800+2	  ;
align 4 						  ;
Arrow_Buffer:				rd  16*16+2	  ;
align 4 						  ;
Folder_Buffer:				rd  16*16+2	  ;
align 4 						  ;
App_Buffer:				rd  16*16+2	  ;
align 4 						  ;
ListBuffer				rb  1024*15	  ;
align 4 						  ;
VesaBuffer:						  ;
					rb   1024*1024*4  ;
